#include "uthread.h"
#include "types.h"
#include "user.h"



thread_t threads[MAX_THREADS];
int current_tid = 0;

// Forward declaration for thread exit
void thread_exit(void);

// Simple round-robin scheduler
static int next_ready_thread() {
    int next = (current_tid + 1) % MAX_THREADS;
    int i;
    for (i = 0; i < MAX_THREADS; i++) {
        int idx = (next + i) % MAX_THREADS;
        if (threads[idx].state == READY) return idx;
    }
    return current_tid; // fallback if no READY threads
}

// Initialize thread system
void thread_init(void) {
    int i;
    for (i = 0; i < MAX_THREADS; i++) {
        threads[i].state = UNUSED;
        threads[i].tid = i;
        threads[i].fn = 0;
        threads[i].arg = 0;
        threads[i].joined_by = -1;
        threads[i].context[0] = 0;
        threads[i].joined_by = -1;
    }
    threads[0].tid = 0;
    threads[0].state = RUNNING;
    current_tid = 0;
}
static void thread_entry(void) {
    thread_t *t = &threads[current_tid];
    t->fn(t->arg);
    thread_exit();
}
// Create a new thread
int thread_create(void (*fn)(void*), void *arg) {
    int tid = -1, i;
    for (i = 0; i < MAX_THREADS; i++) {
        if (threads[i].state == UNUSED) { tid = i; break; }
    }
    if (tid == -1) return -1;
    threads[tid].tid = tid;
    threads[tid].state = READY;
    threads[tid].fn = fn;
    threads[tid].arg = arg;
    threads[tid].joined_by = -1;
    threads[tid].stack = sbrk(STACK_SIZE);
    if ((int)threads[tid].stack == -1) return -1;
    void **sp = (void**)(((uint)threads[tid].stack + STACK_SIZE) & ~15);
    sp[-1] = (void*)thread_entry;
    sp[-2] = 0;
    sp[-3] = 0;
    sp[-4] = 0;
    sp[-5] = 0;
    threads[tid].context[0] = sp - 5;
    return tid;
}

// Yield control to next ready thread
void thread_yield(void) {
    int next_tid = next_ready_thread();
    if (next_tid == current_tid) {
        return;
    }

    int prev_tid = current_tid;
    current_tid = next_tid;

    threads[prev_tid].state = (threads[prev_tid].state == RUNNING) ? READY : threads[prev_tid].state;
    threads[current_tid].state = RUNNING;

    // Pass array as pointer to first element
    context_switch(threads[prev_tid].context, threads[current_tid].context);
}

// Join a thread
int thread_join(int tid) {
    if (threads[tid].state == UNUSED) return -1;

    while (threads[tid].state != ZOMBIE) {
        threads[tid].joined_by = current_tid;
        thread_yield();
    }

    free(threads[tid].stack);
    threads[tid].stack = 0;
    threads[tid].state = UNUSED;
    return tid;
}

void thread_exit(void) {
    int i, any_alive = 0;
    threads[current_tid].state = ZOMBIE;
    for (i = 0; i < MAX_THREADS; i++)
        if (threads[i].state == READY || threads[i].state == RUNNING)
            any_alive++;
    if (any_alive <= 1) exit();
    thread_yield();
}
