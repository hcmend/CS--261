Name: Hailey Mendenhall
ONID: 934509755

Description: My program combines dynamic arrays and linked lists, two types of data structures, to create a 
hash table, which is a third type of data structure. Hash tables store key-value pairs and use a hash
function to translate each key into a specific index in an array. To handle cases where multiple keys are 
hashed to the same value, my program uses chaning. Each index in the dynamic array contains a linked list,
so if a key-value pair already exists at that index, the new pair is simply added to the linked list. 
Additionally, my function resizes the hash table to maintain efficiency. When the number of elements reaches 
four times the number of indices, the table gains more indices and linked lists to accomodate the growing data.


Instructions: Once this program starts, it does not require any input from the user, so the instructions are 
pretty simple. 

    compile: make
    run: valgrind ./test_ht

Limitations: this program only works if the testing function is set up correctly, so that is one limitation. 
However, within the program it should be able to handle most things, besides failed memory allocation since 
it does not require any input from the user. 