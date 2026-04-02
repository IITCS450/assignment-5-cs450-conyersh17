#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread.h"
#include "umutex.h"

#define N 8              // buffer size
#define PRODUCE_COUNT 10 // items per producer
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

static int buf[N], head, tail, count;
static umutex_t mu;

// Producer function
static void producer(void *arg){
    int id = (int)arg;
    for(int i = 0; i < PRODUCE_COUNT; i++){
        int produced = 0;
        while(!produced){
            mutex_lock(&mu);
            if(count < N){
                buf[tail] = id*1000 + i; // unique value per producer
                tail = (tail + 1) % N;
                count++;
                produced = 1;
                printf(1, "Producer %d produced %d (count=%d)\n", id, buf[(tail+N-1)%N], count);
            }
            mutex_unlock(&mu);
            thread_yield(); // cooperative switch
        }
    }
}

// Consumer function
static void consumer(void *arg){
    int id = (int)arg;
    int got = 0;
    while(got < PRODUCE_COUNT * NUM_PRODUCERS / NUM_CONSUMERS){
        int consumed = 0;
        mutex_lock(&mu);
        if(count > 0){
            int x = buf[head];
            head = (head + 1) % N;
            count--;
            got++;
            consumed = 1;
            if(got % 10 == 0 || got == 1)
                printf(1, "Consumer %d consumed %d (count=%d, got=%d)\n", id, x, count, got);
        }
        mutex_unlock(&mu);
        if(!consumed) thread_yield();
    }
}

int main(void){
    thread_init();
    mutex_init(&mu);

    tid_t producers[NUM_PRODUCERS];
    tid_t consumers[NUM_CONSUMERS];

    // Create producers
    for(int i = 0; i < NUM_PRODUCERS; i++){
        producers[i] = thread_create(producer, (void*)(i+1));
    }

    // Create consumers
    for(int i = 0; i < NUM_CONSUMERS; i++){
        consumers[i] = thread_create(consumer, (void*)(i+1));
    }

    // Wait for all producers to finish
    for(int i = 0; i < NUM_PRODUCERS; i++){
        thread_join(producers[i]);
    }

    // Wait for all consumers to finish
    for(int i = 0; i < NUM_CONSUMERS; i++){
        thread_join(consumers[i]);
    }

    printf(1, "test_pc: all producers and consumers finished.\n");
    exit();
}