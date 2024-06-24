#include "threads.h"


void* myfunc(void* args) 
{
    printf("hello world!\n");
    return NULL;
}


void call(void)
{
    pthread_t pt; 
    pthread_create(&pt, NULL, myfunc, NULL); 
    pthread_join(pt, NULL); 


}