/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Hailey Mendenhall
 * Email: mendenhh@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"
#include "stack.h"


/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  struct bst* tree = malloc(sizeof(struct bst)); //allocates the memory
  tree->root = NULL;   //assigns null to the bst_node within bst
  return tree; //returns the pointer to this tree
}

void free_helper(struct bst_node* node){
  if(node == NULL){ //if we have reached the end of left or right subtree, no more to do there. 
    return;
  }
  free_helper(node->left); //frees left side
  free_helper(node->right); // frees right side
  free(node); //after it finished removing everything after that node, it can remove the node
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {
  free_helper(bst->root); // calls helper function to recursively free all elements
  free(bst); //frees bst structure 
  return;
}


int bst_size_helper(struct bst_node* node){
  if (node == NULL){  
    return 0; //returns zero if node passed in is empty. 
  }
  return 1 + bst_size_helper(node->left) + bst_size_helper(node->right); //otherwsie returns 1 for current node + left subtree + right subtree
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {
  if (bst == NULL || bst->root == NULL){ //checks if bst itself has not been initialized or if it's root has not been initialized
    return 0; // if so, its an empty tree, so size is zero
  }
  return bst_size_helper(bst->root);  //otherwise calls the function that will recursively find size
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
  //allocates space for new node and sets it up with info from parameters
  //starts its left and right pointers to null because it may be a leaf
  struct bst_node* new_node = malloc(sizeof(struct bst_node));
  new_node -> key = key;
  new_node -> value = value;
  new_node -> left = NULL;
  new_node->right = NULL;
  if (bst->root == NULL){  //checks if tree is empty
    bst->root = new_node; //then the root is just the new node we made
    return;
  }
  struct bst_node* current = bst->root; //starts out current at root
  struct bst_node* parent = NULL; // keeps track of parent
  while(current != NULL){
    parent = current; //sets parents to current
    if (key < current->key){ 
      current = current->left;  //moves current left if key is smaller than currents    
    }else{
      current = current->right; //moves current right if key is greater than currents
    }
  }
  //now that while loop is done, we have the correct parent for the new_node
  if(key < parent->key){
    parent->left = new_node; //if key is smaller than parents goes on left side
  }else{
    parent->right = new_node; //if key is greater, goes to its right
  }
  return;
}

//this function finds the minimum value in the subtree of node passed in  
struct bst_node* min_value_node(struct bst_node* node){
  struct bst_node* current = node; //sets current to node passed in 
  while(current != NULL && current->left != NULL){ //checks if current is null or there is no value to left (checks if no value smaller)
    current = current->left; // if that is not true, move current left, (move current to smaller node)
  }
  return current; //returns the smallest node
}

//this function is a helper of the remove function. makes it so that it can be called recursively. 
struct bst_node* remove_helper(struct bst_node* root, int key){
  if (key<root->key){ //checks if key is on left of node passes in 
    root->left = remove_helper(root->left, key); //if so, call function again with left value of node
  }else if (key>root->key){ //checks if key is on right of node passed in 
    root->right = remove_helper(root->right,key); //if so, call function again with right value of node
  }else{ //else means it was not bigger or smaller, so we have found the node to remove.

    // case 1: node has one child or no children
    if(root->left == NULL){ //checks if does not have smaller child
      struct bst_node* temp = root->right; //then temp is right child
      free(root); //frees 
      return temp;
    }else if(root->right == NULL){
      struct bst_node* temp = root->left;
      free(root);
      return temp; 
    }

    // case 2: node has two children
    //must find inorder successor
    struct bst_node* temp = min_value_node(root->right);
    root->key = temp->key;
    root->value = temp->value;
    root->right = remove_helper(root->right, temp->key);
  }
  return root;
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
  bst->root = remove_helper(bst->root, key); 
  return;
}



/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
struct bst_node* current = bst->root; //sets current to root
while(current != NULL){ //goes through this until we have reached end of line where that key would be 
  if (key == current -> key){
    return current->value; //returns value of current if the keys match
  }else if(key < current->key){
    current = current->left; //if key is less than currents checks left subtree
  }else{
    current = current->right; //goes to right node cause then key must be bigger than currents
  }
}

  return NULL; // if it has not found the key in that while loop, it does not exist 
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

int maxDepth(struct bst_node* root){
  if (root == NULL){ 
    return -1; //-1 if no values in tree
  }else{
    //recursively calls this function to find depth of left and right subtree;
    int leftDepth = maxDepth(root->left); 
    int rightDepth = maxDepth(root->right);
    if (leftDepth > rightDepth){ //returns whatever one is larger
      return leftDepth + 1;             
    }else{
      return rightDepth + 1;
    }
  }    
}

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
 int bst_height(struct bst* bst) {  
  return maxDepth(bst->root); //calls max depth, starting by passing in the root
 }

//this function helps see if that path is correct, and allows recursion to be used
int bst_path_helper(struct bst_node* node, int target, int current_sum){
 if(node == NULL){
  return 0; //returns 0 if have found end of that subtree
 }
 current_sum += node->key; //adds current node to the sum
 if(node->left == NULL && node->right == NULL && current_sum == target){
  return 1; //if no children, meaning we have found a leaf, and the sum is the path we wanted return 1 cause it was found
 }
 if(bst_path_helper(node->left, target, current_sum) == 1){ //checks if this function finds the right path from left subtree
  return 1;
 }else if(bst_path_helper(node->right, target, current_sum) == 1){ //checks if this function finds the right path from right subtree
  return 1;
 }else{
  return 0; //otherwise it has not been found
 }
 
}
/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
  return bst_path_helper(bst->root, sum, 0);
}

//this fucntion helps find the sum of keys in the range
int bst_range_helper(struct bst_node* node, int lower, int upper){
  if (node == NULL){ //if we are at the end of that path, return 0
    return 0;
  }
  int sum = 0; //start sum at zero
  if (node->key >= lower && node->key <= upper){ //if key is within both lower and upper, add it
    sum += node->key;
  }
  if (node->key < upper){ //if key if less than upper limit, check if right value is still in range
    sum += bst_range_helper(node->right, lower, upper); //if it is, add it
  }
  if(node->key > lower){ //if key is greater than lower limit, check if left value is still in range
    sum += bst_range_helper(node->left, lower, upper);
  }
  return sum;  
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
  return bst_range_helper(bst->root, lower, upper);
}


















/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack* stack;
};

/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */
struct bst_iterator* bst_iterator_create(struct bst* bst) {
  /*
   * FIXME:
   */
  return NULL;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator* iter) {
  /*
   * FIXME:
   */
  return;
}

/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator* iter) {
  /*
   * FIXME:
   */
  return 0;
}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator* iter, void** value) {
  /*
   * FIXME:
   */
  if (value) {
    *value = NULL;
  }
  return 0;
}
