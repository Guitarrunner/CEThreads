#include <stdlib.h>
#include <stdio.h>
#include "./references/queue.h"

void threadq_init(threadq_t *this){
  this->front = NULL;
  this->back = NULL;
  this->nItems = 0;
}

void threadq_enqueue(threadq_t* this, threadq_item item){
  threadq_node_t* node;
  node = (threadq_node_t*) malloc(sizeof(threadq_node_t));
  node->item = item;
  node->next = NULL;
  if(this->back == NULL)
    this->front = node;
  else
    this->back->next = node;
  this->back = node;
  this->nItems++;
}

void threadq_push(threadq_t* this, threadq_item item){
  threadq_node_t* node;
  node = (threadq_node_t*) malloc(sizeof(threadq_node_t));
  node->item = item;
  node->next = this->front;
  if(this->back == NULL)
    this->back = node;
  this->front = node;
  this->nItems++;
}

int threadq_size(threadq_t* this){
  return this->nItems;
}

int threadq_isempty(threadq_t *this){
  return this->nItems == 0;
}

threadq_item threadq_pop(threadq_t* this){
  threadq_item ans;
  threadq_node_t* node;
  if(this->front == NULL){
    fprintf(stderr, "Error: No elements in queue.\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
  node = this->front;
  ans = node->item;
  this->front = this->front->next;
  if (this->front == NULL) this->back = NULL;
  free(node);
  this->nItems--;
  return ans;
}

void threadq_cycle(threadq_t* this){
  if(this->back == NULL)
    return;
  this->back->next = this->front;
  this->back = this->front;
  this->front = this->front->next;
  this->back->next = NULL;
}

threadq_item threadq_front(threadq_t* this){
  if(this->front == NULL){
    fprintf(stderr, "No elements in queue.\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
  return this->front->item;
}

void threadq_destroy(threadq_t* this){
  while(!threadq_isempty(this))
    threadq_pop(this);
}