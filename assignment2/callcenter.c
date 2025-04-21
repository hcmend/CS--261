#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"


/*
 * Define your call struct, and call center struct here.
 */

struct call {
  int ID;
  char* name;
  char* reason;
};

struct call_center{	
	struct queue* queue;
	struct stack* stack; 
	int calls_answered;
	int calls_in_queue;
};

void free_call(struct call* c){
	free(c->name);
	free(c->reason);
	free(c);
}

void* create_call_center(){
	struct call_center* cc = malloc(sizeof(struct call_center));
	cc->queue = queue_create();
	cc->stack = stack_create();
	cc->calls_answered = 0;
	cc->calls_in_queue = 0;
	return cc;
}

void print_options(){
	printf("\n 1. Receive a new call ");
	printf("\n 2. Answer a call ");
	printf("\n 3. Current state of the stack - answered calls ");
	printf("\n 4. Current state of the queue - calls to be answered ");
	printf("\n 5. Quit ");
}

int get_option(int input){
	printf("\n Choose an option: ");
	scanf("%d", &input);
	return input;
}

void receive_call(struct call_center* cc){
	struct call* new_call = malloc(sizeof(struct call));
	new_call->name = malloc(50 * sizeof(char));
	new_call->reason = malloc(50 * sizeof(char));	
	printf("\n Enter the caller name: ");
	scanf("%s", new_call->name);	
	printf(" Enter the call reason: ");
	scanf("%s", new_call-> reason);	
	new_call->ID = cc->calls_in_queue + 1;
	queue_enqueue(cc->queue, new_call);
	cc->calls_in_queue++;
	return;
}

void answer_call(struct call_center* cc){
	if(queue_isempty(cc->queue) == 1){
		printf("\n No call to answer.\n");
		return;
	}
	struct call* answered_call = queue_dequeue(cc->queue);
	stack_push(cc->stack, answered_call);
	cc->calls_answered ++;
	cc->calls_in_queue --;
	printf("\n Call answered and added to stack.\n");
}

void check_stack(struct call_center* cc){
	if(cc->calls_answered == 0){
		printf("\n No calls have been answered.");
		return;
	}
	struct call* last_call = stack_top(cc->stack);
	printf("\n Number of calls answered: %d", cc->calls_answered);
	printf("\n Last call answered: \n ID: %d, Name: %s, Reason: %s\n", last_call-> ID, last_call->name, last_call->reason);	
}

void check_queue(struct call_center* cc){
	if(cc->calls_in_queue == 0){
		printf("\n No calls waiting to be answered.");
		return;
	}
	struct call* to_be_answered = queue_front(cc->queue);
	printf("\n Number of calls waiting to be answered: %d", cc->calls_in_queue);
	printf("\n First call to be answered: \n ID: %d, Name: %s, Reason: %s\n", to_be_answered->ID, to_be_answered->name, to_be_answered->reason);
}

void goodbye(){
	printf("\n Thank you for participating in the call center! Good bye \n");
}

void correct_option(int input, struct call_center* cc){
	if(input == 1){
		receive_call(cc);
	}else if (input == 2){
		answer_call(cc);
	}else if (input == 3){
		check_stack(cc);		
	}else if (input == 4){
		check_queue(cc);
	}else{
		goodbye();
	}
}

int main(int argc, char *argv[]) {
	struct call_center* cc = create_call_center();
	int input = 0;
	while(input != 5){
		printf("\n");
		print_options();
		input = get_option(input);
		correct_option(input, cc);
	}
	while(!stack_isempty(cc->stack)){
		free_call(stack_pop(cc->stack));
	}
	stack_free(cc->stack);
	queue_free(cc->queue);
	free(cc);	
	return 0;
}
