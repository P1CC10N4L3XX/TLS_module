#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/mman.h>


#define DEBUG if(0)

extern int TLS_SIZE;
extern unsigned long tls_position(void);

pthread_key_t tls_key;

extern int __real_pthread_create(pthread_t *thread, 
                    const pthread_attr_t *attr,
                    void *(*start_routine) (void *),
                    void *arg);

typedef struct {
    void *(*the_start_routine)(void *);
    void *the_arg;
}target;


void *tls_allocate(void *);

extern int __wrap_pthread_create(pthread_t *thread,
                            const pthread_attr_t *attr,
                            void *(*start_routine) (void *),
                            void *arg){

    target *trg;
    int ret;

    trg = (target *) malloc(sizeof(target));

    trg->the_start_routine = start_routine;
    trg->the_arg = arg;

    ret = __real_pthread_create(thread, attr, tls_allocate, (void *)trg);

    return ret;

}


void *tls_allocate(void *args){
    void *(*the_start_routine)(void *);
    void *the_arg;

    unsigned long *addr = (unsigned long *)mmap(NULL, TLS_SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);

    if(addr == MAP_FAILED){
        perror("mmap allocation error\n");
        exit(EXIT_FAILURE);
    }


    pthread_setspecific(tls_key, addr);
    
    target *trg = (target *)args;

    the_start_routine = trg->the_start_routine;
    the_arg = trg->the_arg;

    return the_start_routine(the_arg);
}

void tls_destructor(void *ptr){
    if(ptr!=NULL){
        munmap(ptr, TLS_SIZE);
    }
}


void tls_init(void){
    pthread_key_create(&tls_key, tls_destructor);
}




