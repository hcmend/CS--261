/*
 * This file contains the definition of the interface for a dynamic array.
 * You can find descriptions of the dynamic array functions, including their
 * parameters and their return values, in dynarray.c.
 */

#ifndef __DYNARRAY_H
#define __DYNARRAY_H

/*
 * Structure used to represent a dynamic array.  You may not change the fact
 * that only a forward declaration of the dynamic array structure is included
 * here.  In other words, you can't define the fields of the struct here.
 */
struct dynarray;

/*
 * Dynamic array interface function prototypes.  Refer to dynarray.c for
 * documentation about each of these functions.
 */
struct dynarray* dynarray_create();

/*
 * Frees all the memory associated with the dynamic array
 */
void dynarray_free(struct dynarray* da);

/*
 * Returns the amount of elements currently in the array
 */
int dynarray_size(struct dynarray* da);

/*
 * Returns the current capacity of the array
 */
int dynarray_capacity(struct dynarray* da);

/*
 * returns the current start of array with circular buffer. so what index the queue actually starts at
 */
int dynarray_start(struct dynarray* da);

/*
 * inserts a value into the array at the correct index. correct spot is first empty spot in queue. 
 */
void dynarray_insert(struct dynarray* da, void* val);

/*
 * this removes the front of queue.
 */
void dynarray_remove(struct dynarray* da, int idx);

/*
 * this gets the value at a specific index of queue
 */
void* dynarray_get(struct dynarray* da, int idx);

/*
 * this sets the value of an index of array. .
 */
void dynarray_set(struct dynarray* da, int idx, void* val);

void dynarray_print(struct dynarray* da, void (*p) (void* a));


#endif
