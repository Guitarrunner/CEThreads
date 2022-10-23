#ifndef _CETHREAD_H
#define _CETHREAD_H  

#include<queue.h>
#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<unistd.h>
#include<stdbool.h> 
#include<signal.h>
#include<string.h>
#include<sys/types.h>
#include<sys/time.h>

#define STACK_SIZE 512 * 1024
#define QUANTUM 1000

#define RUNNING 0
#define DONE 1
#define ZOMBIE 	2

/*  
*   GTThread--A User Level Thread Library
*   This library is a modification obtained at the website https://github.com/LancelotGT/gtthread
*   Creator: Ning Wang. Nov 23th, 2019.
*/

typedef unsigned long int CEthread_t;

typedef struct{
    
} CEthread_attr_t;

typedef struct{
	bool lock;
	unsigned int id;
	CEthread_t blocking_thread;
} CEthread_mutex_t;

typedef struct{
	
} CEthread_mutexattr_t;

typedef struct{
	ucontext_t * context;
	int state;
	void * retval;
	CEthread_t joining;
	bool joinable;
	CEthread_attr_t * attr;
	CEthread_t id;
} CEthread;

int  CEthread_create(CEthread_t * thread, const CEthread_attr_t * attr, void *(*routine) (void*), void * arg);
void CEthread_end(void * retval);
int  CEthread_yield(void);
int  CEthread_join(CEthread_t thread, void **retval);
int  CEthread_detach(CEthread_t thread);
void CEthread_mutex_init(CEthread_mutex_t *restrict mutex, const CEthread_mutexattr_t *restrict attr);
void CEthread_mutex_destroy(CEthread_mutex_t *mutex);
void CEthread_mutex_unlock(CEthread_mutex_t *mutex);
void CEthread_mutex_trylock(CEthread_mutex_t *mutex);

#endif