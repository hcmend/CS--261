/*
 * This file is where you should implement your queue.  It already contains
 * skeletons of the functions you need to implement (along with documentation
 * for each function).  Feel free to implement any additional functions you
 * might need.  Also, don't forget to include your name and @oregonstate.edu
 * email address below.
 *
 * Name: Hailey Mendenhall
 * Email: mendenhh@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>


#include "queue.h"
#include "dynarray.h"

/*
 * This is the structure that will be used to represent a queue.  This
 * structure specifically contains a single field representing a dynamic array
 * that should be used as the underlying data storage for the queue.
 *
 * You should not modify this structure.
 */
struct queue {
  struct dynarray* array;
};

/*
 * This function should allocate and initialize a new, empty queue and return
 * a pointer to it.
 */
struct queue* queue_create() {
	struct queue* q = malloc(sizeof(struct queue)); // allocates memory for queue. 
	q->array = dynarray_create(); //calls its array to create itself with pre defined function
	return q;	
}

/*
 * This function should free the memory associated with a queue.  While this
 * function should up all memory used in the queue itself, it should not free
 * any memory allocated to the pointer values stored in the queue.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   queue - the queue to be destroyed.  May not be NULL.
 */
void queue_free(struct queue* queue) {
	dynarray_free(queue->array);// calls array to free itself
	free(queue); //frees queue 
  	return;
}

/*
 * This function should indicate whether a given queue is currently empty.
 * Specifically, it should return 1 if the specified queue is empty (i.e.
 * contains no elements) and 0 otherwise.
 *
 * Params:
 *   queue - the queue whose emptiness is being questioned.  May not be NULL.
 */
int queue_isempty(struct queue* queue) {
	if(queue == NULL || queue->array == NULL){ // checks if queue is either not initialized at all or its array hasnt been
		return 1;
	}
	if(dynarray_size(queue->array) == 0){ // checks if queue is empty(array has no values)
		return 1;
	}
	return 0;
}

/*
 * This function should enqueue a new value into a given queue.  The value to
 * be enqueued is specified as a void pointer.  This function must have O(1)
 * average runtime complexity.
 *
 * Params:
 *   queue - the queue into which a value is to be enqueued.  May not be NULL.
 *   val - the value to be enqueued.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void queue_enqueue(struct queue* queue, void* val) {
	if (queue == NULL || queue->array == NULL){ //checks if queue or it's array hasnt been set up yet
		return;
	}
	dynarray_insert(queue->array, val); // calls insert function of array
	return;
}

/*
 * This function should return the value stored at the front of a given queue
 * *without* removing that value.  This function must have O(1) average runtime
 * complexity.
 *
 * Params:
 *   queue - the queue from which to query the front value.  May not be NULL.
 * 
 * Return:
 *   This function should return the front value in the queue.
 */
void* queue_front(struct queue* queue) {
	if(queue == NULL || queue->array == NULL){ // checks if queue or it's array have not been set up yet. cause if so it does not have a first value
		return NULL; 
	}
	return dynarray_get(queue->array, 0); // calls array to get the value, and passes in 0 for first index.
}

/*
 * This function should dequeue a value from a given queue and return the
 * dequeued value.  This function must have O(1) average runtime complexity.
 *
 * Params:
 *   queue - the queue from which a value is to be dequeued.  May not be NULL.
 *
 * Return:
 *   This function should return the value that was dequeued.
 */
void* queue_dequeue(struct queue* queue) {
	if(queue == NULL || queue->array == NULL){ // checks if queue or it's array have not been initialized yet. if so canr remove a value. 
		return NULL;
	}
	void* val_removed = dynarray_get(queue->array, 0); // grabs value that will be removed to be returned later
	dynarray_remove(queue->array, 0); // calls array's remove function
	return val_removed;	
}


/*
 * helper function for testing, do not modify
 */

/*
 * This function print out the queue. (for testing circular buffer)
 * Params:
 *   queue - the queue from which a value is to be print.  May not be NULL.
 *	 void (*p) (void* a) - a function pointer to print an element within the queue
 * Return:
 *   none.
 */
void queue_print(struct queue* queue, void (*p) (void* a)){
	if (queue == NULL)
		return;

	dynarray_print(queue->array, p); 
	
	return;
}