/*
 * TestStack.c
 *
 * Very simple unit test file for Stack functionality.
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "Stack.h"
#include "myassert.h"


#define MAX_STACK_SIZE 20

/*
 * The stack to use during tests
 */
static Stack *stack;

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
	stack = new_Stack(MAX_STACK_SIZE);
	total_count++;
}

/*
 * Teardown function to run after each test
 */
void teardown(){
	Stack_destroy(stack);
}

/*
 * This function is called multiple times from main for each user-defined test function
 */
void runTest(int (*testFunction)()) {
	setup();

	if (testFunction()) success_count++;

	teardown();
}



int newStackIsNotNull() {
	assert(stack != NULL);
	return TEST_SUCCESS;
}

/*
 * Checks that the size of an empty stack is 0.
 */
int newStackSizeZero() {
	assert(Stack_size(stack) == 0);
	return TEST_SUCCESS;
}

/*
 * check the functionality when pushing one element
 * check if the element is poped successfully by
 * checking the return flag of pop function
 *          the size of the stack in incremented
 */
int pushOneElement(){
	int* PNumber1 = malloc(sizeof(int));
	assert(Stack_push(stack,PNumber1) == true);
	assert(Stack_size(stack) == 1);
	return TEST_SUCCESS;
}

/* 
 * false flag need to be returned if null element poped
 * nothing is inserted so size of stack is still zero
 */
int pushNullElement(){
	void* nullElement = NULL;
	assert(Stack_push(stack,nullElement) == false);
	assert(Stack_size(stack) == 0);
	return TEST_SUCCESS;
}

/*
 * push multiple elements into stack
 * the maximum size flag return true until its reaching maximum size
 * checking if stack has already keeping maximum size of element
 */
int pushTooMuch(){
	for(int i=0;i<MAX_STACK_SIZE;i++){
		int* PNumber1 = malloc(sizeof(int));
		assert(Stack_push(stack,PNumber1) == true);
	}
	int* PNumber1 = malloc(sizeof(int));
	assert(Stack_push(stack,PNumber1) == false);
	assert(Stack_size(stack) == MAX_STACK_SIZE);
	return TEST_SUCCESS;	
}

/*
 * pop the element that pushed into the stack
 * check if they are identical by comparing their pointer
 */
int popOneElement(){
	int* PNumber1 = malloc(sizeof(int));
	*PNumber1 = 1;
	Stack_push(stack,PNumber1);
	void* PElement = Stack_pop(stack);
	assert(PElement == PNumber1);
	assert((int*)PElement == (int*)PNumber1);
	assert(Stack_size(stack)==0);
	return TEST_SUCCESS;
}

int popEmptyStack(){
	assert(Stack_pop(stack) == NULL);
	return TEST_SUCCESS;
}

/*
 * check if number of pop action over the maximum size
 * return null element
 */
int popTooMuch(){
	int* PNumber1 = malloc(sizeof(int));
	Stack_push(stack,PNumber1);
	Stack_pop(stack);
	assert(Stack_pop(stack) == NULL);
	return TEST_SUCCESS;
}

/*
 * check the functionality of poping multiple elements
 * stack follows the rule of first in last out
 * pop function should only take the last element pushed into stack
 * and the size of stack need to be reduced every time poping
 */
int popMultipleElements(){
	int* PNumber1 = malloc(sizeof(int));
	int* PNumber2 = malloc(sizeof(int));
	int* PNumber3 = malloc(sizeof(int));
	Stack_push(stack,PNumber1);
	Stack_push(stack,PNumber2);
	Stack_push(stack,PNumber3);
	assert(Stack_pop(stack) == PNumber3);
	assert(Stack_size(stack)== 2);
	assert(Stack_pop(stack) == PNumber2);
	assert(Stack_size(stack)== 1);
	assert(Stack_pop(stack) == PNumber1);
	assert(Stack_size(stack)== 0);
	return TEST_SUCCESS;
}

/*
 * check the function of clearing stack
 * every list node need to be cleared
 * size is cleared to zero
 * nothing can be poped out
 */
int stackisCleared(){
	for(int i=0;i<MAX_STACK_SIZE;i++){
		int* PNumber1 = malloc(sizeof(int));
		assert(Stack_push(stack,PNumber1) == true);
	}
	Stack_clear(stack);
	assert(Stack_size(stack)==0);
	assert(Stack_pop(stack)==NULL);
	return TEST_SUCCESS;
}



int main() {
	runTest(newStackIsNotNull);
	runTest(newStackSizeZero);
	runTest(pushOneElement);
	runTest(pushNullElement);
	runTest(pushTooMuch);
	runTest(popOneElement);
	runTest(popEmptyStack);
	runTest(popTooMuch);
	runTest(popMultipleElements);
	printf("Stack Tests complete: %d / %d tests successful.\n----------------\n", success_count, total_count);

}
