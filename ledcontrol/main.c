// https://www.geeksforgeeks.org/multithreading-c-2/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

#include "ledcontrol.h"
#include "ledgui.h"

// sem_t semTest;

void *Task1 (void *vargp);

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
    pthread_create(&thread_id2, NULL, LedControllerTask, NULL); 
    pthread_create(&thread_id, NULL, LedThread, NULL); 
    pthread_create(&thread_id3, NULL, Task1, NULL); 
    pthread_join(thread_id, NULL); 
    pthread_join(thread_id2, NULL); 
    // pthread_join(thread_id3, NULL); 
    exit(0); 
}

void *Task1 (void *vargp)
{
    // tLedPattern ledPatternCanOpen;
    printf("Printing from Task 1\n");
    while (semLedRequest == NULL) 
    {
        Task_sleep_ms(1);
    }
    // Turn on Power LED
    {
        tLedPattern ledPattern1;

        ledPattern1.colorPattern[0] = LED_COLOR_RED;
        ledPattern1.ui8PatternSize = 1;
        ledPattern1.ui8DelayTicks = 0;
        ledPattern1.ledFrequency = LED_FREQ_5Hz;
        ledPattern1.ui8Count = LED_INFINITE_REPETITION;

        Semaphore_post(semLedRequest);      // make sure its open
        LED_SetLedPattern(LED_ID_0, &ledPattern1);

    }
    return NULL; 
}