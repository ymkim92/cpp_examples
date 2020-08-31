#include <stdio.h>
#include <assert.h>
#include "ledcontrol.h"

#include <time.h>
#include <assert.h>
#include <errno.h>

#define DEBUG_ENABLED
#define PFSER                           printf
#define System_printf                   printf
#define MILLISECONDS_TO_TICKS(m)        (m)
#define LED_TASK_PERIOD_MS              20

#define LED_COLOR_RED       0
#define LED_COLOR_GREEN     1
#define LED_COLOR_YELLOW    2
#define LED_COLOR_MIX1      LED_COLOR_YELLOW
#define LED_COLOR_OFF       3
#define LED_COLOR_COUNT     4

#define LED_R       1
#define LED_G       2

#define LED_PATTERN_READY               0xff
#define LED_INFINITE_REPETITION         0xff

typedef struct LedStateControl
{
    // If the ui8PatternIndex is LED_PATTERN_READY, it means that it is the first in processing
    uint32_t    ui32Time;
    uint8_t     ui8PatternIndex;
    uint8_t     ui8DelayTicksIndex;
    bool        bEndOfPatternCycle;
    bool        bEndOfRepetition;
}
tLedStateControl;

typedef struct LedControl
{
    tLedPattern ledPattern;
    tLedStateControl ledStateControl;
    bool        bValidPendingPattern;
    tLedPattern ledPendingPattern;
}
tLedControl;


bool newLedRequest[LED_ID_COUNT];
static bool newLedPendingRequest[LED_ID_COUNT];

uint8_t testLedBlink;
uint8_t ui8LedSleepMode;

tLedControl ledControl[LED_ID_COUNT];
tLedControl protectedLedControl[LED_ID_COUNT];
static uint32_t pui32Interval[6];
sem_t semLedRequest[] = {0};
bool    bLedReady = false;


void LED_InitModes();
void LED_InitPattern();
static void LED_BlinkTest();
static void LED_DisplayColorToAllLeds(uint8_t a_Color);
static uint32_t ErrReg_GetErrors();
static void WD_Pet();
static uint32_t Clock_getTicks();
static uint32_t GetIntervalFromFreq(uint8_t ui8LedFrequency);
uint8_t LED_DisplayPattern(tLedControl *a_LedControl, uint8_t a_i, uint8_t a_PatternIndex);
static void GPIO_write(int color, int value);
void LED_SetColor(uint8_t a_i, uint8_t a_LedColor);
void Task_sleep_ms(int ms);

void *LedControllerTask(void *vargp)
{
	printf("Starting LedController task: \n");

    LED_InitModes();
    LED_InitPattern();

#ifdef DEBUG_ENABLED
    testLedBlink = 1;  // we test LED on reboot
#endif

    while (1)
    {
        uint32_t ui32TimeDiff, ui32CurrTime;
        uint8_t i, patternIndex;

        if (ui8LedSleepMode != 0)
        {
            Task_sleep_ms(10);
            continue;
        }

        if (testLedBlink != 0)
        {
            LED_BlinkTest();
//            Semaphore_post(semLeds); // because LED_SetColor() consumes semLeds
            testLedBlink = 0;
        }

#if defined(SENSOR_TYPE_TSI4) || defined(SENSOR_TYPE_TSI4_R2)
        // process a special case - there is a major failure.
        if (ErrReg_GetErrors() & 0xffffffc0)
        {
            LED_SetColor(LED_ID_0, LED_COLOR_RED);
            Task_sleep_ms(250);
            WD_Pet(WD_LED);
            continue;
        }
#endif

        for (i=0; i<LED_ID_COUNT; i++)
        {
            if (Semaphore_pend(semLedRequest, MILLISECONDS_TO_TICKS(LED_TASK_PERIOD_MS)) == true) // we can't block forever, because of watchdog
            {
                // === take a snapshot
                if (newLedRequest[i] == true)
                {
                    protectedLedControl[i] = ledControl[i];
                    newLedRequest[i] = false;
                }
                if (newLedPendingRequest[i] == true)
                {
                    protectedLedControl[i].ledPendingPattern = ledControl[i].ledPendingPattern;
                    protectedLedControl[i].bValidPendingPattern = true;

                    newLedPendingRequest[i] = false;
                }

                // === open the data collection callback
                Semaphore_post(semLedRequest);

                 // === blink logic
                 patternIndex = protectedLedControl[i].ledStateControl.ui8PatternIndex;
                 // for the first of a new pattern
                 if (patternIndex == LED_PATTERN_READY)
                 {
                     PFSER("new pattern %d\r\n", i);

                     patternIndex = LED_DisplayPattern(&protectedLedControl[i], i, 0);
                     protectedLedControl[i].ledStateControl.ui32Time = Clock_getTicks();
                 }
                 else
                 {
#ifndef DEBUG_ENABLED
                     assert(patternIndex <= LED_PATTERN_MAX);
#endif

                     // Frequency handling
                     ui32CurrTime = Clock_getTicks();
                     ui32TimeDiff = ui32CurrTime - protectedLedControl[i].ledStateControl.ui32Time;
                     if (ui32TimeDiff >= GetIntervalFromFreq(protectedLedControl[i].ledPattern.ledFrequency))
                     {
                         // for the new pending pattern
                         //
                         if ((protectedLedControl[i].ledStateControl.bEndOfPatternCycle
                                     && (protectedLedControl[i].bValidPendingPattern)))
                         {
                             PFSER("new pending pattern %d\r\n", i);
                             // move pending pattern to the pattern
                             protectedLedControl[i].ledPattern = protectedLedControl[i].ledPendingPattern;
                             protectedLedControl[i].bValidPendingPattern = false;
                             protectedLedControl[i].ledStateControl.bEndOfPatternCycle = false;
                             protectedLedControl[i].ledStateControl.bEndOfRepetition = false;

                             patternIndex = LED_DisplayPattern(&protectedLedControl[i], i, 0);
                         }
                         // End of Repetition
                         else if (protectedLedControl[i].ledStateControl.bEndOfRepetition == true)
                         {
                             PFSER("end of repetition %d\r\n", i);

                             // Turn off LED
                             LED_SetColor(i, LED_COLOR_OFF);
                         }
                         // End of Pattern
                         else if (patternIndex >= protectedLedControl[i].ledPattern.ui8PatternSize)
                         {
                             PFSER("end of pattern %d\r\n", i);

                             // End of Cycle
                             if (protectedLedControl[i].ledStateControl.ui8DelayTicksIndex >=
                                     protectedLedControl[i].ledPattern.ui8DelayTicks)
                             {
                                 PFSER("end of cycle %d\r\n", i);

                                 // if not an infinite loop
                                 if (protectedLedControl[i].ledPattern.ui8Count != LED_INFINITE_REPETITION)
                                 {
                                     protectedLedControl[i].ledPattern.ui8Count--;
                                     if (protectedLedControl[i].ledPattern.ui8Count == 0)
                                     {
                                         protectedLedControl[i].ledStateControl.bEndOfRepetition = true;
                                     }
                                 }
                                 protectedLedControl[i].ledStateControl.ui8DelayTicksIndex = 0;
                                 patternIndex = 0;
                                 protectedLedControl[i].ledStateControl.bEndOfPatternCycle = true;

                                 // for the new pending pattern
                                 //
                                 if ((protectedLedControl[i].ledStateControl.bEndOfPatternCycle
                                             && (protectedLedControl[i].bValidPendingPattern)))
                                 {
                                     PFSER("new pending pattern %d\r\n", i);
                                     // move pending pattern to the pattern
                                     protectedLedControl[i].ledPattern = protectedLedControl[i].ledPendingPattern;
                                     protectedLedControl[i].bValidPendingPattern = false;
                                     protectedLedControl[i].ledStateControl.bEndOfPatternCycle = false;
                                     protectedLedControl[i].ledStateControl.bEndOfRepetition = false;

                                     patternIndex = LED_DisplayPattern(&protectedLedControl[i], i, 0);
                                 }


                                 // End of Repetition
                                 else if (protectedLedControl[i].ledStateControl.bEndOfRepetition == true)
                                 {
                                     PFSER("end of repetition %d\r\n", i);

                                     // Turn off LED
                                     LED_SetColor(i, LED_COLOR_OFF);
                                 }
                                 else
                                 {
                                     patternIndex = LED_DisplayPattern(&protectedLedControl[i], i, 0);
                                 }

                             }
                             // Display delayTicks
                             else
                             {
                                 // turn off LED
                                 LED_SetColor(i, LED_COLOR_OFF);
                                 protectedLedControl[i].ledStateControl.ui8DelayTicksIndex++;
                             }
                         }
                         else
                         {
                             if (patternIndex == 0)
                             {
                                 PFSER(".");
                             }
                             //assert(patternIndex != 0);
                             patternIndex = LED_DisplayPattern(&protectedLedControl[i], i, patternIndex);
                         }
                         protectedLedControl[i].ledStateControl.ui32Time = ui32CurrTime;

                     }
                 }
                 protectedLedControl[i].ledStateControl.ui8PatternIndex = patternIndex;

                 bLedReady = true;
            }
            else
            {
                Semaphore_post(semLedRequest); // sem_pend timeout, but stil need to release the sem
            }
        } // for (i=0; i<LED_ID_COUNT; i++)

        // IMPORTANT!!!
        // This sleep time should be less than 50ms, because the highest LED frequence is 20Hz
        Task_sleep_ms(25);


        WD_Pet(WD_LED);
    }

}

void LED_SetColor(uint8_t a_i, uint8_t a_LedColor)
{
    if (a_LedColor >= LED_COLOR_COUNT)
    {
        System_printf("Color: %d\r\n", a_LedColor);
    }
    assert(a_LedColor < LED_COLOR_COUNT);

    if (a_LedColor == LED_COLOR_RED)
    {
    	GPIO_write(LED_R, 1);
    	GPIO_write(LED_G, 0);
    }
    else if (a_LedColor == LED_COLOR_GREEN)
    {
    	GPIO_write(LED_R, 0);
    	GPIO_write(LED_G, 1);
    }
    else if (a_LedColor == LED_COLOR_YELLOW)
    {
    	GPIO_write(LED_R, 1);
    	GPIO_write(LED_G, 1);
    }
    else if (a_LedColor == LED_COLOR_OFF)
    {
    	GPIO_write(LED_R, 0);
    	GPIO_write(LED_G, 0);
    }
}


void LED_InitModes()
{
    // Turn off LED
    LED_SetColor(LED_ID_0, LED_COLOR_OFF);
}

void LED_InitPattern()
{
    uint8_t i;

    for (i=0; i<LED_ID_COUNT; i++)
    {
        protectedLedControl[i].ledStateControl.ui8PatternIndex = 1;
        protectedLedControl[i].ledStateControl.bEndOfRepetition = true;
        protectedLedControl[i].ledStateControl.bEndOfPatternCycle = true;
    }

    pui32Interval[0] = MILLISECONDS_TO_TICKS(50);
    pui32Interval[1] = MILLISECONDS_TO_TICKS(100);
    pui32Interval[2] = MILLISECONDS_TO_TICKS(200);
    pui32Interval[3] = MILLISECONDS_TO_TICKS(400);
    pui32Interval[4] = MILLISECONDS_TO_TICKS(800);
    pui32Interval[5] = MILLISECONDS_TO_TICKS(1600);
}

static void LED_BlinkTest()
{
    // blink
    //

    LED_DisplayColorToAllLeds(LED_COLOR_OFF);
    Task_sleep_ms(1000);
    LED_DisplayColorToAllLeds(LED_COLOR_GREEN);
    Task_sleep_ms(1000);
    LED_DisplayColorToAllLeds(LED_COLOR_RED);
    Task_sleep_ms(1000);
    LED_DisplayColorToAllLeds(LED_COLOR_OFF);
    Task_sleep_ms(1000);
}

static void LED_DisplayColorToAllLeds(uint8_t a_Color)
{
    uint8_t i;

    for (i=0; i<LED_ID_COUNT; i++)
    {
        // set color
        LED_SetColor(i, a_Color);
    }
}

static void WD_Pet()
{

}

static uint32_t ErrReg_GetErrors()
{
    return 0;
}

bool Semaphore_pend(sem_t *sem, int timeoutMs)
{
    int ret;
    struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        printf("RT clock error!!\n");
    }

    ts.tv_nsec += 20000000;
    ts.tv_sec += ts.tv_nsec / 1000000000;
    ts.tv_nsec %= 1000000000;

    while ((ret = sem_timedwait(sem, &ts)) == -1 && errno == EINTR)
        continue;
        
    return true;
}

void Semaphore_post(sem_t *sem)
{
    if (sem_post(sem) == -1) {
        static const char err_msg[] = "sem_post() failed\n";
        printf(err_msg);
    }
}

static uint32_t Clock_getTicks()
{
    return 0;
}

static uint32_t GetIntervalFromFreq(uint8_t ui8LedFrequency)
{
    return pui32Interval[ui8LedFrequency];
}

uint8_t LED_DisplayPattern(tLedControl *a_LedControl, uint8_t a_i, uint8_t a_PatternIndex)
{

#ifndef DEBUG_ENABLED
    if (a_PatternIndex == 0)
    {
        assert(a_LedControl.ledPattern.ui8Count != 0);
        assert(a_LedControl.ledPattern.ui8PatternSize != 0);
        assert(a_LedControl.ledPattern.ui8PatternSize < LED_PATTERN_READY);
    }
#endif

    // display the pattern
    LED_SetColor(a_i, a_LedControl->ledPattern.colorPattern[a_PatternIndex]);
    a_PatternIndex++;
    return a_PatternIndex;
}

static void GPIO_write(int color, int value)
{

}

void Task_sleep_ms(int ms)
{

}

bool LED_SetLedPattern(tLedId a_LedId, tLedPattern *a_pLedPattern)
{
    bool ret;
    //Semaphore_pend(semLedRequest, BIOS_WAIT_FOREVER);
    if (semLedRequest == NULL)
    {
        return false;
    }

    ret = Semaphore_pend(semLedRequest, MILLISECONDS_TO_TICKS(LED_TASK_PERIOD_MS)); // we can't block forever, because of wathcdog
    if (ret == true) {
// #if defined(SENSOR_TYPE_TSI4) || defined(SENSOR_TYPE_TSI4_R2)
//         if(calibrationCRCError()) {
//             ledControl[a_LedId].ledPattern = emergencyPatern;
//         }
//         else
// #endif
            ledControl[a_LedId].ledPattern = *a_pLedPattern;

        ledControl[a_LedId].bValidPendingPattern = false;
        ledControl[a_LedId].ledStateControl.ui8PatternIndex = LED_PATTERN_READY;
        ledControl[a_LedId].ledStateControl.ui8DelayTicksIndex = 0;
        ledControl[a_LedId].ledStateControl.bEndOfPatternCycle = false;
        ledControl[a_LedId].ledStateControl.bEndOfRepetition = false;
        newLedRequest[a_LedId] = true;
    }
    Semaphore_post(semLedRequest);

    return ret;
}

bool LED_SetLedPendingPattern(tLedId a_LedId, tLedPattern *a_pLedPattern)
{
    bool ret;
    //Semaphore_pend(semLedRequest, BIOS_WAIT_FOREVER);
    if (semLedRequest == NULL)
    {
        return false;
    }

    ret = Semaphore_pend(semLedRequest, MILLISECONDS_TO_TICKS(LED_TASK_PERIOD_MS)); // we can't block forever, because of wathcdog
    if (ret == true) {
        ledControl[a_LedId].ledPendingPattern = *a_pLedPattern;
        ledControl[a_LedId].bValidPendingPattern = true;
        newLedPendingRequest[a_LedId] = true;
    }
    Semaphore_post(semLedRequest);
    return ret;
}

bool LED_StopLedPattern(tLedId a_LedId)
{
    bool ret;
    //Semaphore_pend(semLedRequest, BIOS_WAIT_FOREVER);
    ret = Semaphore_pend(semLedRequest, MILLISECONDS_TO_TICKS(LED_TASK_PERIOD_MS)); // we can't block forever, because of wathcdog
    if (ret == true) {
        ledControl[a_LedId].ledPattern.colorPattern[0] = LED_COLOR_OFF;
        ledControl[a_LedId].ledPattern.ui8PatternSize = 1;
        ledControl[a_LedId].ledPattern.ui8DelayTicks = 0;
        ledControl[a_LedId].ledPattern.ledFrequency = LED_FREQ_5Hz;
        ledControl[a_LedId].ledStateControl.ui8PatternIndex = LED_PATTERN_READY;
        ledControl[a_LedId].ledStateControl.ui8DelayTicksIndex = 0;
        newLedRequest[a_LedId] = true;
    }
    Semaphore_post(semLedRequest);
    return ret;
}