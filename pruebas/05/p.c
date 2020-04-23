#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct params {
        pthread_mutex_t mutex;
        int id;
};

typedef struct params params_t;

void* hello(void* arg){
    int id;
    pthread_mutex_lock(&(*(params_t*)(arg)).mutex);
    id = (*(params_t*)(arg)).id;
    pthread_mutex_unlock(&(*(params_t*)(arg)).mutex);
    printf("Hello from %d\n", id);
}


int main(int argc, char *argv[]) {
    pthread_t threads[10];
    params_t params;
    pthread_mutex_init (&params.mutex , NULL);

    int i;
    for(i = 0; i < 10; i++) {
            params.id = i;
            if(pthread_create(&threads[i], NULL, hello, &params));
    }

    for(i = 0; i < 10; i++) {
            pthread_join(threads[i], NULL);
    }

    return 0;
}
