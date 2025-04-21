/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Hailey Mendenhall
 * Email: mendenhh@oregonstate.edu
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"



/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */

struct pq_node{
	int priority; //priority value of node
	void* data; //node's value
};

struct pq{
	struct dynarray* nodes; //dynamic array
	int size;	//size of queue
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* queue = malloc(sizeof(struct pq)); //allocates memory
	queue->nodes = dynarray_create(); //calls array to create itself and assigns that to nodes
	queue->size = 0; //queue starts as empty
	return queue;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue. That is the responsibility of the caller. However, you
 * do need to free the memory that holds each element stored in the priority 
 * queue. 
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	for(int i = 0; i < pq->size; i++){ //goes through all indices
		free(dynarray_get(pq->nodes, i)); //calls dynarray get for every index and frees that node
	}
	dynarray_free(pq->nodes); //frees dyn array
	free(pq); //frees priority queue as a whole
	return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if(pq->size == 0){
		return 1;
	}
	return 0;
}

//this function swaps the two nodes passed in to it
//I use it as a helper function for adding and removing from priority queue
void swap(struct pq_node* a, struct pq_node* b){
	struct pq_node temp = *a;
	*a = *b;
	*b = temp;
}

//this function ensures that the heap is still in tact after inserting a value into the first open spot in pq
void keep_heap(struct dynarray* da, int index){
	int parent_index = (index-1) /2; //computes index of parent of index passed in
	struct pq_node* current_node = dynarray_get(da, index); //gets current node
	struct pq_node* parent_node = dynarray_get(da, parent_index); //gets parent node
	while(index > 0 && parent_node->priority > current_node->priority){		//while parents priority is bigger then its child
		swap(parent_node, current_node);	//if that while loop is fulfilled, must swap to keep min heap		 	
		keep_heap(da, parent_index); //recursively calls itself		
	}
}

/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	//allocates memory for new node and sets info passed in to it
	struct pq_node* new_node = malloc(sizeof(struct pq_node)); 
	new_node->priority = priority;
	new_node->data = value;

	dynarray_insert(pq->nodes, new_node); //calls dynarray to insert the node to end
	pq->size ++; //ups size
	keep_heap(pq->nodes, pq->size -1);	//makes sure we still have a min heap after adding node
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	struct pq_node* first_node = dynarray_get(pq->nodes, 0); //grabs first node from array
	return first_node->data; //returns it's value
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	struct pq_node* first_node = dynarray_get(pq->nodes, 0); //grabs first node from array
	return first_node->priority; //returns its priority
}

void place_value (struct pq* pq, int index){
	//computes indices of left and right child of index passed in
	int left_child = (2 * index) + 1; 
	int right_child = (2 * index) + 2;	
	int smallest = index; //starts index w/ smallest value at what was passed in
	
	if (left_child < pq->size){ //if left child is within array (i.e. it exists)
		struct pq_node* left = dynarray_get(pq->nodes, left_child); //gets the node at that index
		struct pq_node* current = dynarray_get(pq->nodes, index); //get node at index passed in
		if(left->priority < current->priority){ //if left childs priority is less than currents, new smallest is there 
			smallest = left_child; 
		}
	}
	if (right_child < pq->size){ // if right child is within array i.e. exists
		struct pq_node* right = dynarray_get(pq->nodes, right_child); //gets node at that index
		struct pq_node* smallest_node = dynarray_get(pq->nodes, smallest); //gets node at index w/ current smallest, either what was passed in or left child
		if (right->priority < smallest_node->priority){ //if right is smaller than that it is now smallest
			smallest = right_child;
		}
	}		
	if(smallest != index){	 //if index for smallest was changed during either of last if statements, need to swap nodes. 	
		swap (dynarray_get(pq->nodes, index), dynarray_get(pq->nodes, smallest));
		place_value(pq, smallest); //recursive call
	}
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	if(pq_isempty(pq) == 1){ //if nothing to remove exit function
		return NULL;
	}	
	struct pq_node* first_node = dynarray_get(pq->nodes, 0); //get first node
	void* first = pq_first(pq); //get first node's value
	free(first_node); //free the info at that first node
	struct pq_node* last_node = dynarray_get(pq->nodes, pq->size - 1); //get last node
	dynarray_set(pq->nodes, 0, last_node); //move last node to first spot
	dynarray_remove(pq->nodes, pq->size -1); //remove last node, dont need it anymore
	//free(last_node); //free that node
	pq->size --; //decrement size
	if (pq_isempty(pq) != 1){ //if that was not the only node need to make sure we still have min heap and potentially move that node we placed at the root down
		place_value(pq, 0);
	}
	return first;
}
