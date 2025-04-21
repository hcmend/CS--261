/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Hailey Mendenhall
 * Email: mendenhh@oregonstate.edu
 */

#include <stdlib.h>
#include <string.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"
#include <stdio.h>

//struct for hash table node
struct ht_node{
  //key-value pair. 
  void* key;
  void* value;
  struct ht_node* next;
};

/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */
struct ht{
    struct dynarray* buckets;
    int size;
};     

/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* table = malloc(sizeof(struct ht)); //allocates space
    table->buckets = dynarray_create(); //calls dynarray create to initialize its buckets
    for(int i = 0; i < dynarray_cap(table->buckets); i++){ //loops through the initial capacity of dynarray
        struct list* lst = list_create(); //calls list create for all every index 
        dynarray_insert(table->buckets, lst); //inserts lists into the array
    }
    table -> size = 0; //starts size at 0
    return table;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    if(ht != NULL){ //checks if ht is already NULL
        for(int i = 0; i < dynarray_cap(ht->buckets); i++){ //loops though every index in array
            struct list* lst = dynarray_get(ht->buckets, i); //gets lists at that index
            struct node* current = list_head(lst); //starts at the head
            while(current){ //while current node is not null                
                struct node* next = list_next(current); //grabs the next node
                //gets the ht_node pointed to by that node and frees it
                struct ht_node* ht_node = (struct ht_node*)list_get_val(current); 
                free(ht_node->key);
                free(ht_node->value);
                free(ht_node); 
                current = next; //moves current along list
            }
            list_free(lst); //frees list at index i
        }
        dynarray_free(ht->buckets); //after looping through, frees whole dynarray
        free(ht); //frees ht struct
    }  
    return;
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){
   return ht->size == 0; 
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 *
 * Params:
 *   ht - the hash table whose size is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return the size of the given hash table.
 */
int ht_size(struct ht* ht){    
   return ht->size;    
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash function is passed in as a function pointer, stored in 'convert'
 * Assuming the passed in hash function is well designed.
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element used to calculate the index
 *   convert - a pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the index value of 'key' in the hash table .
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
    int hashcode = convert(key); //uses convert function passed in to hash the key 
    int index = hashcode % dynarray_cap(ht->buckets); //makes sure that the value is returned is an index within the array 
    return index;
}

//this is a helper function for inserting a ht_node. it is called when the load factor is 4 or greater. 
void ht_resize(struct ht* ht, int (*convert)(void*)){
    //gets the current capacity of array and the capacity we want to set it at
    int old_capacity = dynarray_cap(ht->buckets);
    int new_capacity = old_capacity * 2;
 
    // creates new dynamic array with increased capacity
    struct dynarray* new_buckets = dynarray_create();        
    _dynarray_resize(new_buckets, new_capacity);
    
    //loops through that new array and inserts an empty list at every index
    for (int i = 0; i < new_capacity; i++) {
        struct list* lst = list_create();       
        dynarray_insert(new_buckets, lst);        
    }

    //loops though old array and copies everything over 
    for (int i = 0; i < old_capacity; i++) {
        struct list* lst = dynarray_get(ht->buckets, i); //gets list at that index
        struct node* current = list_head(lst); //starts at head
        while (current) { //so it ends when we reach NULL
            struct node* next = list_next(current);
            struct ht_node* ht_node = (struct ht_node*)list_get_val(current);  //find ht_node at current node          
            int new_index = convert(ht_node->key) % new_capacity; //computes where it should be in new array
            struct list* new_lst = dynarray_get(new_buckets, new_index); //grabs list at that index just computed from new array            
            struct ht_node* new_ht_node = malloc(sizeof(struct ht_node)); //creates a new ht_node to put all the info in to
            new_ht_node->key = ht_node->key;
            new_ht_node->value = ht_node->value;
            new_ht_node->next = NULL;
            list_insert(new_lst, new_ht_node); //inserts it into the list at the right index of our new array
            current = next; //moves current along
            free(ht_node); //frees the ht_node from old array
        }
        list_free(lst); //frees list from old array
    }
    dynarray_free(ht->buckets); //frees old array
    ht->buckets = new_buckets;  //updates buckets 
}

//this is also a helper function for inserting and looking up a node. it compares two void* keys passed in 
int ht_compare_keys(void* key1, void* key2){
    //first, must cast the void* to ints. 
    int int_key1 = *(int*)key1;
    int int_key2 = *(int*)key2;
    //return whether the keys are equal or not
    if(int_key1 == int_key2){ 
        return 0;
    }
    return -1;
}

//this is another helper function for insertion. it checks if the the key we are inserting is already in the hash table
int check_for_key_match(struct ht* ht, void* key, void* value){
    for(int i = 0; i < dynarray_cap(ht->buckets); i++){  //must loop through whole array to check everywhere for match   
        struct list* lst = dynarray_get(ht->buckets, i); //grabs list at index i
        struct node* current = list_head(lst); //starts at head
        while(current){ //goes until reaches null           
            struct ht_node* ht_node = (struct ht_node*)list_get_val(current); //grabs ht_node pointed to by current      
            if(ht_compare_keys(ht_node->key, key) == 0){  //calls compare keys to see if they are equal                             
                ht_node->value = value; //if they are, just update the value of that ht_node and exit loops                
                return 0;            
            }  
            current = list_next(current);   //otherwise, keep moving current along to continue search for match 
        } 
    }    
    //if it made it through loops w/o returning, there was no match for key
    return 1;   
}

/*
 * This function should insert a given element into a hash table with a
 * specified key. Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){   
    
    //computes the load factor
    int cap = dynarray_cap(ht->buckets); 
    int load_factor = ht->size / cap;     

    //checks if the key is already in hash table
    if (check_for_key_match(ht, key, value) == 0) {        
        return; //if so, do not need to insert new node
    }

    // resizes if load factor exceeds the threshold
    if (load_factor >= 4) {       
        ht_resize(ht, convert);        
    }

    //gets the index that we should insert at and the list at that index
    int index = ht_hash_func(ht, key, convert); 
    struct list* lst = dynarray_get(ht->buckets, index);    

    //allocates space for new node to be inserted and assigns all info passed in to it
    struct ht_node* new_ht_node = malloc(sizeof(struct ht_node));
    new_ht_node->key = key;
    new_ht_node->value = value;
    new_ht_node->next = NULL;
    
    //inserts it into the list we grabbed from correct index
    list_insert(lst, new_ht_node);
    
    //must update size
    ht->size++;    
}
       

/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the value of the corresponding 'key' in the hash table .
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
    //find the index that ht_node would be at
    int index = ht_hash_func(ht, key, convert);
    //grabs list at that index
    struct list* lst = dynarray_get(ht->buckets, index);
    struct node* current = list_head(lst); //starts at head
    while(current){ //goes until reach null
        struct ht_node* ht_node = (struct ht_node*)list_get_val(current); //gets ht_node pointed to by current
        if (ht_compare_keys(ht_node->key, key) == 0){ //checks if the key passed in and that ht_nodes key are a match
            return ht_node->value; //if they are we found what we want to return
        }
        current = list_next(current); //if not found yet, keep moving current along list
    }    
    return NULL; //if it reaches this point, it went through everything and there was no match
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){
    //finds what index that ht_node would be in
    int index = ht_hash_func(ht, key, convert);
    //grabs the list at that index
    struct list* lst = dynarray_get(ht->buckets, index);
    struct node* current = list_head(lst); //starts at the head 
    struct node* prev = NULL; //starts prev at null
    while(current){ //goes until current is null
        struct ht_node* ht_node = (struct ht_node*)list_get_val(current); //gets ht_node pointed to by current
        if (ht_compare_keys(ht_node->key, key) == 0){ //sees if their keys are equal
            //if so, we found what we want to remove
            if (prev){
                list_set_next(prev, list_next(current)); // if there is a previous node, need to update previous' next 
            }else{
                list_set_head(lst, list_next(current)); //if no prev, we removed the head and need to update it
            }
            free(ht_node); //frees node we want to remove
            
            ht->size--; //updates size
            return; //after removal done, should return
        }
        prev = current; //if  not found match yert, must keep moving prev and current along 
        current = list_next(current);
    }
    return;
} 
