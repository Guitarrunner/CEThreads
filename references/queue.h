#ifndef THREADQ_H
#define THREADQ_H

typedef void* threadq_item;

typedef struct threadq_node_t{
  threadq_item item;
  struct threadq_node_t* next;
} threadq_node_t;

typedef struct {
  threadq_node_t* front;
  threadq_node_t* back;
  int nItems;
} threadq_t;


void threadq_init(threadq_t* this);
int threadq_isempty(threadq_t* this);
int threadq_size(threadq_t* this);
void threadq_enqueue(threadq_t* this, threadq_item item);
void threadq_push(threadq_t* this, threadq_item item);
threadq_item threadq_pop(threadq_t* this);
void threadq_cycle(threadq_t* this);
threadq_item threadq_front(threadq_t* this);
void threadq_destroy(threadq_t* this);

#endif