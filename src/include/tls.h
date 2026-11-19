#ifndef TLS
#define TLS

#define PER_THREAD_MEMORY_START typedef struct _TLS_ZONE{\
    unsigned long tls_address;
#define PER_THREAD_MEMORY_END }TLS_ZONE;\
    int TLS_SIZE = sizeof(TLS_ZONE);


void *tls_get(void){
    void *tls_pointer;
    __asm__ volatile (
        "mrs %0, tpidr_el0":
        "=r"(tls_pointer)
    );
    return tls_pointer;
}


#define READ_THREAD_VARIABLE(var)\
    ((TLS_ZONE*)(tls_get()))->var

#define WRITE_THREAD_VARIABLE(var, value)\
    ((TLS_ZONE*)(tls_get()))->var = value
#endif