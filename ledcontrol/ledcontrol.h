#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

#define SENSOR_TYPE_TSI4

#define LED_PATTERN_MAX             240

#define LED_FREQ_20Hz       0       // 50ms
#define LED_FREQ_10Hz       1       // 100ms
#define LED_FREQ_5Hz        2       // 200ms
#define LED_FREQ_2_5Hz      3       // 400ms
#define LED_FREQ_1_25Hz     4       // 800ms
#define LED_FREQ_0_625Hz    5       // 1.6s

typedef struct LedPattern
{
    uint8_t colorPattern[LED_PATTERN_MAX];
    uint8_t ui8PatternSize;
    uint8_t ui8DelayTicks;                  // for the ledFrequency
    uint8_t ledFrequency;
    uint8_t ui8Count;                       // use LED_INFINITE_REPETITION for infinite repetition
}
tLedPattern;

typedef enum LedId
{
    LED_ID_0,   // CANopen, Error status
    LED_ID_COUNT,
}
tLedId;

typedef enum Task_WD_
{
#if defined(SENSOR_TYPE_TSI4) || defined(SENSOR_TYPE_TSI4_R2)
    WD_MURATA,
    WD_CAN,
    WD_LED,
#elif defined SENSOR_TYPE_LSI3
    // TODO enable this after adding MURATA in LSi3
//    WD_MURATA,
    WD_LASER,
    WD_CAN,
    WD_LED,
#elif defined SENSOR_TYPE_RPI3F
    WD_UBLOX_GPS_CONFIG,
    WD_UBLOX_GPS_DATA,
    WD_CAN,
    WD_LED,
#else
    WD_BLE,
    WD_BLUETOOTH,
    WD_BATTERY_MONITOR,
    WD_GPS_OR_MM,
    WD_CAN,
    WD_LED,
#endif
    WD_SIZE
} Task_WD_t;

void *LedControllerTask(void *vargp);
bool Semaphore_pend(sem_t *sem, int timeoutMs);
void Semaphore_post(sem_t *sem);

/**
 * LED_SetLedPattern
 *
 * set and go LED Pattern to a certain LED
 *
 * @param a_LedId       the index of a certain LED. e.g., LED 0
 * @param a_LedPattern  the LED pattern
 *
 * @return      true
 */
bool LED_SetLedPattern(tLedId a_LedId, tLedPattern *a_pLedPattern);

/**
 * LED_StopLedPattern
 *
 * Turn off the LED specified by parameter a_LedId
 *
 * @param a_LedId       the index of a certain LED. e.g., LED 0
 *
 * @return      true
 */
bool LED_StopLedPattern(tLedId a_LedId);

/**
 * LED_SetLedPendingPattern
 *
 * PendingPattern will be applied
 * after the running cycle (pattern + delayTicks) of the current pattern ends
 *
 * @param a_LedId       the index of a certain LED. e.g., LED 0
 * @param a_LedPattern  the LED pattern
 *
 * @return      true
 *
 */
bool LED_SetLedPendingPattern(tLedId a_LedId, tLedPattern *a_pLedPattern);

/**
 * LED_SetTestLed
 *
 * Once this is called, all LEDs stop displaying colors, show the green and red LED at all LEDs,
 * then, go back to normal state
 *
 * @return
 *              LED_TEST_MODE_OK
 *              LED_TEST_MODE_ALREADY_RUNNING
 *
 */
uint8_t LED_SetTestLed();

#ifdef __cplusplus
} // extern "C"
#endif