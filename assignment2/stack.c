/*
 * This file is where you should implement your stack.  It already contains
 * skeletons of the functions you need to implement (along with documentation
 * for each function).  Feel free to implement any additional functions you
 * might need.  Also, don't forget to include your name and @oregonstate.edu
 * email address below.
 *
 * Name: Hailey Mendenhall
 * Email: mendenhh@oregonstate.edu
 */

#include <stdlib.h>

#include "stack.h"
#include "list.h"

/*
 * This is the structure that will be used to represent a stack.  This
 * structure specifically contains a single field representing a linked list
 * that should be used as the underlying data storage for the stack.
 *
 * You should not modify this structure.
 */
struct stack {
  struct list* list;
};

/*
 * This function should allocate and initialize a new, empty stack and return
 * a pointer to it.
 */
struct stack* stack_create() {
	struct stack* s = malloc(sizeof(struct stack)); // allocating array
	s->list = list_create(); // calls it's list to create itself in predefined function
	return s; 
}

/*
 * This function should free the memory associated with a stack.  While this
 * function should up all memory used in the stack itself, it should not free
 * any memory allocated to the pointer values stored in the stack.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   stack - the stack to be destroyed.  May not be NULL.
 */
void stack_free(struct stack* stack) { 
	list_free(stack->list); // calls its list to free itself
	free(stack); // frees stack
	return;
}

/*
 * This function should indicate whether a given stack is currently empty.
 * Specifically, it should return 1 if the specified stack is empty (i.e.
 * contains no elements) and 0 otherwise.
 *
 * Params:
 *   stack - the stack whose emptiness is being questioned.  May not be NULL.
 */
int stack_isempty(struct stack* stack) {
	if(stack == NULL || stack->list == NULL || list_return_first(stack->list) == NULL){ // checks if stack or it's list have not been initialized or if the first element is null
		return 1;
	}
	return 0;
}

/*
 * This function should push a new value onto a given stack.  The value to be
 * pushed is specified as a void pointer.  This function must have O(1)
 * average runtime complexity.
 *
 * Params:
 *   stack - the stack onto which a value is to be pushed.  May not be NULL.
 *   val - the value to be pushed.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void stack_push(struct stack* stack, void* val) {
	if(stack == NULL){
		return;
	}
	list_insert(stack->list, val); // pushes element on stack by calling its list to insert itself 
	return;
}

/*
 * This function should return the value stored at the top of a given stack
 * *without* removing that value.  This function must have O(1) average runtime
 * complexity.
 *
 * Params:
 *   stack - the stack from which to query the top value.  May not be NULL.
 */
void* stack_top(struct stack* stack) {
	if(stack == NULL || stack->list == NULL){ // checks if stack or its list is null cause then its first element is not truly set up
		return NULL;
	}
	void* first_value = list_return_first(stack->list); //calls list to return its initial value
	return first_value;
}

/*
 * This function should pop a value from a given stack and return the popped
 * value.  This function must have O(1) average runtime complexity.
 *
 * Params:
 *   stack - the stack from which a value is to be popped.  May not be NULL.
 *
 * Return:
 *   This function should return the value that was popped.
 */
void* stack_pop(struct stack* stack) {
	if(stack == NULL || stack->list == NULL || list_return_first(stack->list) == NULL){ // checks if list has either not been set up or doesnt have any values to remove
		return NULL; 
	}
	void* popped_value = list_return_first(stack->list); // calls list to return first value to save before it gets removed so that it can be returned later
	list_remove_first(stack->list); // calls list to remove first value
	return popped_value;
}
