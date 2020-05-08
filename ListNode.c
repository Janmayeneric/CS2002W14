#include "ListNode.h"


/*
 *
 * basic idea of the ListNode is inspired from the example made by jon1
 * 
 * void pointer is used for storing the different type of data structure in list
 * 
 * it store the pointer to literal struct and another list in the program
 * s
 * 
*/
ListNode *new_ListNode(void* element, ListNode *next){
    ListNode *this = (ListNode*) malloc(sizeof(ListNode));
    this->element = element;
    this->next = next;
    return this;
}

// already commented under the comment of List_destroy
// ListNode_destroy only free the memory allocation of listnode itself
int ListNode_destroy(ListNode *node){
    free(node);
    return 0;
}

// free the memory allocation of the element
// assign null value to the element inside list node
// not destory the list node itself
int ListNode_destroyElement(ListNode *node){
    free(node->element);
    node->element = NULL;
    return 0;
}