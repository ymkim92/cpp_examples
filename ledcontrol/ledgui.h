#include <gtk/gtk.h>

void *LedThread (void *vargp);
void GPIO_write(int led, int color, int value);
void LED_WaitReady();
void Task_sleep_ms(int ms);
