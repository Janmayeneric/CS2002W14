/*
 * TestStack.c
 *
 * Very simple unit test file for BlockingStack functionality.
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "BlockingStack.h"
#include "myassert.h"


#define MAX_STACK_SIZE 20

/*
 * The stack to use during tests
 */
static BlockingStack *stack;

/*
 * The number of tests that succeeded
 */
static int success_count = 0;

/*
 * The total number of tests run
 */
static int total_count = 0;


/*
 * Setup function to run prior to each test
 */
void setup(){
	stack = new_BlockingStack(MAX_STACK_SIZE);
	total_count++;
}

/*
 * Teardown function to run after each test
 */
void teardown(){
	BlockingStack_destroy(stack);
}

/*
 * This function is called multiple times from main for each user-defined test function
 */
void runTest(int (*testFunction)()) {
	setup();

	if (testFunction()) success_count++;

	teardown();
}


/*
 * Checks that the BlockingStack constructor returns a non-NULL pointer.
 */
int newStackIsNotNull() {
	assert(stack != NULL);
	return TEST_SUCCESS;
}

/*
 * Checks that the size of an empty blocking stack is 0.
 */
int newStackSizeZero() {
	assert(BlockingStack_size(stack) == 0);
	return TEST_SUCCESS;
}

/**
 * Lots of functionality of Blocking stack is simliar to stack
 * so only carry out some essential testing
 */
int pushAndPopOneElement(){
	int* PNumber = malloc(sizeof(int));
	BlockingStack_push(stack,PNumber);
	ListNode* pop_list = BlockingStack_pop(stack);

	pthread_join(stack->push_thread,NULL);
	pthread_join(stack->pop_thread,NULL);

	pop_list = pop_list->next;
	assert(PNumber = pop_list->element);
	return TEST_SUCCESS;
}

/**
 * push excess elements into stack
 * pop the elements then
 * all element will be pushed out including 
 */
int pushBusyWaiting(){
	ListNode* pop_list;
	int num_push = MAX_STACK_SIZE * 2;
	int** array_PNumber[num_push];

	for(int i=0;i<num_push;i++){
		int* PNumber = malloc(sizeof(int));
		array_PNumber[i] = PNumber;
		BlockingStack_push(stack,PNumber);
	}
	for(int i=0;i<num_push;i++){
		pop_list = BlockingStack_pop(stack);
	}
	
	pthread_join(stack->pop_thread,NULL);
	pthread_join(stack->push_thread,NULL);

	for(int i=0;i<num_push;i++){
		pop_list = pop_list->next;
		assert(pop_list->element = array_PNumber[i]);
	}
	return TEST_SUCCESS;
}

int main() {
	runTest(newStackIsNotNull);
	runTest(newStackSizeZero);
    runTest(pushAndPopOneElement);
	runTest(pushBusyWaiting);
	printf("\nBlockingStack Tests complete: %d / %d tests successful.\n----------------\n", success_count, total_count);

}
