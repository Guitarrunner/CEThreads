#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct qNode{
	void *data;
	struct qNode *next;
};

struct Cola{
	int size;
	int element_size;
	struct qNode *front;
	struct qNode *rear;
};

// Function for initializing the cola 
int initCola(struct Cola *cola, int element_size);
// Function for creating a qNode 
struct qNode * createqNode(void *element, int element_size);
// Function for checking whether the cola is empty
int isEmpty(struct Cola *cola);
// Inserts an element at rear of cola
int encola(struct Cola *cola, void *element);
// Removes an element from front of cola and gets its value
int desencola(struct Cola *cola, void *target);
// Deallocates the memory allocated to cola 
int destroy(struct Cola *cola);
// Function this function put front to queque
void deenCola(struct Cola *cola);

#endif
