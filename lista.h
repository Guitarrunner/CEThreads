#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct lNode{
	void *data;
	struct lNode *next;
};

struct List{
	int length;
	int element_size;
	struct lNode *head;
	struct lNode *tail;
};

// Function for creating the Linked List
struct List * createList(int element_size);
// Function for creating a lNode
struct lNode * createlNode(void *element, int element_size);
// Function for checking whether the list is empty
int isListEmpty(struct List *list);
// Adds an element to the end of the list
int addAt(struct List *list, int index, void *element);
int addFirst(struct List *list, void *element);
int addLast(struct List *list, void *element);

int getAt(struct List *list, int index, void *target);
int getFirst(struct List *list, void *target);
int getLast(struct List *list, void *target);

int removeAt(struct List *list, int index);
int removeFirst(struct List *list);
int removeLast(struct List *list);

// Deallocates the memory allocated to Linked List
int clear(struct List *list);
int destroyList(struct List *list);
int priority_algorithm(struct List *list, int length, int (*compare)(const void *a, const void *b));
int shortest_job_first_algorithm(struct List *list, int length, int (*compare)(const void *a, const void *b, const void *c), const void *c);
int first_come_first_served_algorithm(struct List *list, int lenght, int (*compare)(const void *a, const void *b, const void *c), const void *c);
void swap(struct lNode *a, struct lNode *b);
void printList(struct List *list, void (*printElement)(const void *a));

#endif
