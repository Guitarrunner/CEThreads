#include "./references/linkedList.h"

struct List * createList(int element_size){
	struct List *list = (struct List *)calloc(1, sizeof(struct List));

	if(list == NULL){
		fprintf(stderr, "Insufficient memory to create linked list.\n");
	    exit(1);
	}
	list->length = 0;
	list->element_size = element_size;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

struct lNode * createlNode(void *element, int element_size){
	struct lNode *new_lNode = (struct lNode *)calloc(1, sizeof(struct lNode));
	if (new_lNode != NULL){
		new_lNode->data = calloc(1, element_size);
		new_lNode->next = NULL;
		memcpy(new_lNode->data, element, element_size);
	}
	return new_lNode;
}

int isListEmpty(struct List *list){
	return list->head == NULL;
}

int addAt(struct List *list, int index, void *element){
	if (isListEmpty(list) || index >= list->length)
		return -1;
	else if (index == 0)
		addFirst(list, element);
	else if (index == list->length - 1)
		addLast(list, element);
	else{
		struct lNode *new_lNode = createlNode(element, list->element_size);
		struct lNode *tmp = list->head;
		for (int i = 0; i < index - 1; i++)
			tmp = tmp->next;
		struct lNode *target = tmp->next;
		tmp->next = new_lNode;
		new_lNode->next = target;
		list->length++;
	}
	return 0;
}

int addFirst(struct List *list, void *element){
	struct lNode *new_lNode = createlNode(element, list->element_size);
	if (isListEmpty(list))
		list->head = list->tail = new_lNode;
	else{
		new_lNode->next = list->head;
		list->head = new_lNode;
	}
	list->length++;
	return 0;
}

int addLast(struct List *list, void *element){
	struct lNode *new_lNode = createlNode(element, list->element_size);
	if (isListEmpty(list))
		list->tail = list->head = new_lNode;
	else{
		list->tail->next = new_lNode;
		list->tail = new_lNode;
	}
	list->length++;
	return 0;
}

int getAt(struct List *list, int index, void *target){
	if (isListEmpty(list) || index >= list->length)
		return -1;
	else if (index == 0)
		memcpy(target, list->head->data, list->element_size);
	else if (index == list->length - 1)
		memcpy(target, list->tail->data, list->element_size);
	else{
		struct lNode *tmp = list->head->next;
		for (int i = 1; i < index; i++)
			tmp = tmp->next;
		memcpy(target, tmp->data, list->element_size);
	}
	return 0;
}

int getFirst(struct List *list, void *target){
	if (isListEmpty(list))
		return -1;
	else
		memcpy(target, list->head->data, list->element_size);
	return 0;
}

int getLast(struct List *list, void *target){
	if (isListEmpty(list))
		return -1;
	else
		memcpy(target, list->tail->data, list->element_size);
	return 0;
}

int removeAt(struct List *list, int index){
	if (isListEmpty(list) || index >= list->length)
		return -1;
	else if (index == 0)
		removeFirst(list);
	else if (index == list->length - 1)
		removeLast(list);
	else{
		struct lNode *tmp = list->head;
		for (int i = 0; i < index - 1; i++)
			tmp = tmp->next;
		struct lNode *target = tmp->next;
		tmp->next = target->next;
		free(target->data);
		free(target);
		list->length--;
	}
	return 0;
}

int removeFirst(struct List *list){
	if (isListEmpty(list))
		return -1;
	else{
		struct lNode *target = list->head;
		list->head = target->next;
		free(target->data);
		free(target);
	}
	list->length--;
	return 0;
}

int removeLast(struct List *list){
	if (isListEmpty(list))
		return -1;
	else if (list->length == 1)
	{
		free(list->tail->data);
		free(list->tail);
		list->tail = NULL;
	}
	else{
		struct lNode *tmp = list->head;
		while (tmp->next != list->tail)
			tmp = tmp->next;
		list->tail = tmp;
		free(tmp->next->data);
		free(tmp->next);
		list->tail->next = NULL;
	}
	list->length--;
	return 0;
}

int clear(struct List *list){
	struct lNode *current_lNode;
	while (list->head != NULL){
		current_lNode = list->head;
		list->head = current_lNode->next;
		free(current_lNode->data);
		free(current_lNode);
	}
	return 0;
}


int destroyList(struct List *list){
	clear(list);
	free(list);
	return 0;
}


void printList(struct List *list, void (*printElement)(const void *a)){
	struct lNode *tmp = list->head;  
	while (tmp != NULL){ 
        	printElement(tmp->data);
        	tmp = tmp->next; 
    	} 
}

int bubbleSort3(struct List *queue, int length, int (*compare)(const void *a, const void *b, const void *c), const void *c){
	if (isListEmpty(queue))
		return -1;
	if (length > queue->length)
		length = queue->length;
	int swapped;
	for (int i = 0; i < length; i++){
		struct lNode *tmp = queue->head;
		swapped = 0;
		for (int j = 0; j < length - 1 - i; j++){
			struct lNode *n = tmp->next;
			if (compare(tmp->data, n->data, c)){
				swap(tmp, n);
				swapped = 1;
			}
			tmp = tmp->next;
		}
		if (swapped == 0)
			break;
	}
	return 0;
}


int bubbleSort2(struct List *queue, int length, int (*compare)(const void *a, const void *b)){
	if (isListEmpty(queue))
		return -1;
	if (length > queue->length)
		length = queue->length;
	int swapped;
	for (int i = 0; i < length; i++){
		struct lNode *tmp = queue->head;
		swapped = 0;
		for (int j = 0; j < length - 1 - i; j++){
			struct lNode *n = tmp->next;
			if (compare(tmp->data, n->data)){
				swap(tmp, n);
				swapped = 1;
			}
			tmp = tmp->next;
		}
		if (swapped == 0)
			break;
	}
	return 0;
}

void swap(struct lNode *a, struct lNode *b){
	void *tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}