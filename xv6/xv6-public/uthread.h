#ifndef UTHREAD_H
#define UTHREAD_H

#define MAX_THREADS 8
#define STACK_SIZE 2048

typedef enum { UNUSED, RUNNING, READY, ZOMBIE } thread_state;

typedef struct thread {
    int tid;
    thread_state state;
    void *stack;
    void (*fn)(void*);      // thread function
    void *arg;              // argument
    void *context[8];       // saved registers for context switch
    int joined_by;          // tid of thread waiting for this one
} thread_t;

// Thread API
void thread_init(void);
int thread_create(void (*fn)(void*), void *arg);
void thread_yield(void);
int thread_join(int tid);

// Internal: context switching
void context_switch(void **old, void **new);
typedef int tid_t;  // thread ID type

void thread_init(void);
int thread_create(void (*fn)(void*), void *arg);
void thread_yield(void);
int thread_join(int tid);


#endif