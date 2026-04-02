#include "types.h"
#include "user.h"
#include "uthread.h"

static void myfunc(void *arg) {
    printf(1, "thread %d running\n", (int)arg);
}

int main(void) {
    thread_init();
    int t1 = thread_create(myfunc, (void*)1);
    printf(1, "created thread %d\n", t1);
    thread_yield();
    printf(1, "back in main\n");
    thread_join(t1);
    printf(1, "done\n");
    exit();
}
