/*
 * Stack.c
 *
 * Fixed-size generic array-based Stack implementation.
 *
 * List.c is implemented by the example code List.c from Author: jon1 
 *
 * only comment on the customized changes based on the data structure 
 * 
 */

#include "Stack.h"

// top is the top of the stack
// it is the last element inserted into the stack
// following the "first in last out" rule
Stack *new_Stack(int max_size) {
	Stack *this = (Stack*)malloc(sizeof(Stack));
	this->current_size = 0;
	this->max_size = max_size;
	this->top = NULL;
	return this;
}

// top list node will point to next list node
// when pushing new element 
// new list node will point to original top list node
// and then set to new list node
bool Stack_push(Stack* this, void* element) {
	// NULL element is not permitted for inserting
	if(element == NULL){
		return false;
	}
	// overlaping the size of the fix-size stack
	if(this->current_size == this->max_size){
		return false;
	}
	this->top = new_ListNode(element,this->top);
	this->current_size++;
	return true;
}

// poping function will check the stack first
// return NULL if there is no element inside
// stack list will reset the top element to the next element of top list node
// then destroy the list node and return the pointer to the element
void* Stack_pop(Stack* this) {
	if(this->current_size == 0){
		return NULL;
	}
	ListNode *popedNode = this->top;
	this->top = this->top->next;
	void *popedElement = popedNode->element;
	ListNode_destroy(popedNode);
	return popedElement;
}

// return the current size of the stack
int Stack_size(Stack* this) {
	return this->current_size;
}


bool Stack_isEmpty(Stack* this) {
	if(this->current_size==0){
		return true;
	}
	return false;
}

// clear the stack from top element to base element with loopoing
// free the memory allocation of list node and element s
void Stack_clear(Stack* this) {
	while(this->current_size > 0){
		ListNode *toBeRemoved = this->top;
		this->top = this->top->next;
		ListNode_destroyElement(toBeRemoved);
		ListNode_destroy(toBeRemoved);
		this->current_size--;
	}
}

// clear the stack first
// then free the memory allocation of stack itself
void Stack_destroy(Stack* this) {
	Stack_clear(this);
	free(this);
}

