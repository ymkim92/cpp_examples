// https://www.geeksforgeeks.org/multithreading-c-2/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

#include "ledcontrol.h"
#include "ledgui.h"

// sem_t semTest;

void *Thread1(void *vargp) 
{ 
    bool ret;
    // ret = Semaphore_pend(&semTest, 1500); // we can't block forever, because of wathcdog
    if (ret == true) 
    {
    // sleep(1); 

    // tLedPattern ledPatternCanOpen;

        printf("Printing from Thread 1\n"); 
    }

    return NULL; 
} 

void *Thread2(void *vargp) 
{ 
    sleep(1); 
    // Semaphore_post(&semTest);
    printf("Printing from Thread 2\n"); 
    return NULL; 
} 
   
int main() 
{ 
    InitSemaphore();
    pthread_t thread_id; 
    pthread_t thread_id2; 
    pthread_t thread_id3; 
    // if (sem_init(&semTest, 0, 0) == -1) {
    //     perror("sem_init");
    //     exit(EXIT_FAILURE);
    // }
    pthread_create(&thread_id3, NULL, LedControllerTask, NULL); 
    pthread_create(&thread_id, NULL, LedThread, NULL); 
    // pthread_create(&thread_id2, NULL, Thread2, NULL); 
    // pthread_join(thread_id, NULL); 
    // pthread_join(thread_id2, NULL); 
    pthread_join(thread_id3, NULL); 
    exit(0); 
}
