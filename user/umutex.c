#include "umutex.h"

// Initialize mutex
void mutex_init(umutex_t *m) {
    m->locked = 0;
}

// Lock mutex (cooperative)
void mutex_lock(umutex_t *m) {
    while (__sync_lock_test_and_set(&m->locked, 1)) {
        thread_yield(); // yield while locked
    }
}

// Unlock mutex
void mutex_unlock(umutex_t *m) {
    m->locked = 0;
}