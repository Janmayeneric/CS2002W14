/*
 * Stack.c
 *
 * Fixed-size generic array-based Stack implementation.
 *
 */

#include <stddef.h>
#include "Stack.h"


/*
 * This functions below all return default values and don't work.
 * You will need to provide a correct implementation of the Stack module interface as documented in Stack.h.
 */


Stack *new_Stack(int max_size) {
	return NULL;
}


bool Stack_push(Stack* this, void* element) {
	return false;
}


void* Stack_pop(Stack* this) {
	return NULL;
}


int Stack_size(Stack* this) {
	return -1;
}


bool Stack_isEmpty(Stack* this) {
	return false;
}


void Stack_clear(Stack* this) {
}


void Stack_destroy(Stack* this) {
}

