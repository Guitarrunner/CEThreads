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

struct List * createList(int element_size);
struct lNode * createlNode(void *element, int element_size);
int isListEmpty(struct List *list);
int addAt(struct List *list, int index, void *element);
int addFirst(struct List *list, void *element);
int addLast(struct List *list, void *element);

int getAt(struct List *list, int index, void *target);
int getFirst(struct List *list, void *target);
int getLast(struct List *list, void *target);

int removeAt(struct List *list, int index);
int removeFirst(struct List *list);
int removeLast(struct List *list);

int clear(struct List *list);
int destroyList(struct List *list);

int bubbleSort2(struct List *list, int length, int (*compare)(const void *a, const void *b));
int bubbleSort3(struct List *list, int length, int (*compare)(const void *a, const void *b, const void *c), const void *c);

void swap(struct lNode *a, struct lNode *b);

void printList(struct List *list, void (*printElement)(const void *a));

#endif