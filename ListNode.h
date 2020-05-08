#ifndef LISTNODE_H
#define LISTNODE_H
#include <stdlib.h>

typedef struct ListNode ListNode;

struct ListNode{
    void* element;
    ListNode *next;
};

ListNode *new_ListNode(void* element, ListNode *next);
int ListNode_destroy(ListNode *node);
int ListNode_destroyElement(ListNode *node);
#endif 