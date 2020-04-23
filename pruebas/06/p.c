#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct params {
        pthread_mutex_t mutex;
        pthread_cond_t done;
        int id;
};

typedef struct params params_t;

void* hello(void* arg){

    int id;
    /* Lock.  */
    pthread_mutex_lock(&(*(params_t*)(arg)).mutex);

    /* Work.  */
    id = (*(params_t*)(arg)).id;
    printf("Hello from %d\n", id);

    /* Unlock and signal completion.  */
    pthread_mutex_unlock(&(*(params_t*)(arg)).mutex);
    pthread_cond_signal (&(*(params_t*)(arg)).done);

    /* After signalling `main`, the thread could actually
    go on to do more work in parallel.  */
}


int main() {

    pthread_t threads[10];
    params_t params;
    pthread_mutex_init (&params.mutex , NULL);
    pthread_cond_init (&params.done, NULL);

    /* Obtain a lock on the parameter.  */
    pthread_mutex_lock (&params.mutex);

    int i;
    for(i = 0; i < 10; i++) {

            /* Change the parameter (I own it).  */    
            params.id = i;

            /* Spawn a thread.  */
            pthread_create(&threads[i], NULL, hello, &params);

            /* Give up the lock, wait till thread is 'done',
            then reacquire the lock.  */
            pthread_cond_wait (&params.done, &params.mutex);
    }

    for(i = 0; i < 10; i++) {
            pthread_join(threads[i], NULL);
    }

    /* Destroy all synchronization primitives.  */    
    pthread_mutex_destroy (&params.mutex);
    pthread_cond_destroy (&params.done);

    return 0;
}
