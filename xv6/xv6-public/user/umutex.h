#ifndef UMUTEX_H
#define UMUTEX_H

#include "uthread.h"

typedef struct {
    int locked;
} umutex_t;

// Mutex API
void mutex_init(umutex_t *m);
void mutex_lock(umutex_t *m);
void mutex_unlock(umutex_t *m);

#endif