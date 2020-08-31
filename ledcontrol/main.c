// https://www.geeksforgeeks.org/multithreading-c-2/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

#include "ledcontrol.h"
  
// A normal C function that is executed as a thread  
// when its name is specified in pthread_create() 
void *Thread1(void *vargp) 
{ 
    sleep(1); 
    printf("Printing from Thread 1\n"); 
    return NULL; 
} 

void *Thread2(void *vargp) 
{ 
    sleep(2); 
    printf("Printing from Thread 2\n"); 
    return NULL; 
} 
   
int main() 
{ 
    LedControllerTask();

    pthread_t thread_id; 
    pthread_t thread_id2; 
    printf("Before Thread\n"); 
    pthread_create(&thread_id, NULL, Thread1, NULL); 
    pthread_create(&thread_id2, NULL, Thread2, NULL); 
    pthread_join(thread_id, NULL); 
    pthread_join(thread_id2, NULL); 
    printf("After Thread\n"); 
    exit(0); 
}
