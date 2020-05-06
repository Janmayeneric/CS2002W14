/*
 * BlockingStack.c
 *
 * Fixed-size generic array-based BlockingStack implementation.
 *
 */

#include <stddef.h>
#include "BlockingStack.h"


/*
 * This functions below all return default values and don't work.
 * You will need to provide a correct implementation of the Stack module interface as documented in BlockingStack.h.
 */


BlockingStack *new_BlockingStack(int max_size) {
	return NULL;
}


bool BlockingStack_push(BlockingStack* this, void* element) {
	return false;
}


void* BlockingStack_pop(BlockingStack* this) {
	return NULL;
}


int BlockingStack_size(BlockingStack* this) {
	return -1;
}


bool BlockingStack_isEmpty(BlockingStack* this) {
	return false;
}


void BlockingStack_clear(BlockingStack* this) {
}


void BlockingStack_destroy(BlockingStack* this) {
}
