#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "src/include/tls.h"

#define WORKERS 2


PER_THREAD_MEMORY_START
    long int x;
    long int y;
    long int z;
PER_THREAD_MEMORY_END



void *the_work(void *arg){
    
    long int i = (long int)arg;

    long int the_first = (i+1) * 2;
    long int the_second = (i+1) * 3;
    WRITE_THREAD_VARIABLE(x, the_first);
    WRITE_THREAD_VARIABLE(y, the_second);
    WRITE_THREAD_VARIABLE(z, the_first + the_second);

    long int real_x = READ_THREAD_VARIABLE(x);
    long int real_y = READ_THREAD_VARIABLE(y);
    long int real_z = READ_THREAD_VARIABLE(z);

    printf("%ld thread is making %ld + %ld = %ld\n", i, real_x, real_y, real_z);
    
    return NULL;

}

int main(){

    pthread_t tid;

    long int i;
    for(i=0; i<WORKERS; i++){
        pthread_create(&tid, NULL, the_work, (void*)i);
    }

    pause();
    
    return 0;
}