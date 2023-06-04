
#include "unity.h"
#include "GPIO.h"
#include "MK20DX256.h"

void test_TheFirst(void)
{
    TEST_IGNORE_MESSAGE("Implement Me");
}

void test_SetPinAsOutput_should_ConfigurePinDirection()
{
    PORTC.PDDR = 0;

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(0));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0), PORTC.PDDR);
    
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(22));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0) | BIT_TO_MASK(22), PORTC.PDDR);

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(31));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0) | BIT_TO_MASK(22) | BIT_TO_MASK(31), PORTC.PDDR);
}

void test_SetPinAsOutput_should_NotUpdatePinDirection_when_PinIsNotValid()
{
    PORTC.PDDR = 0;

    TEST_ASSERT_EQUAL(GPIO_ERROR, GPIO_SetPinAsOutput(32));
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PDDR);
}

void test_SetPinAsInput_should_ConfigurePinDirection()
{
    PORTC.PDDR = 0xffffffff; // all configured to GPIO output

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(0));
    TEST_ASSERT_EQUAL_HEX32(~(BIT_TO_MASK(0)), PORTC.PDDR);
    
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(16));
    TEST_ASSERT_EQUAL_HEX32(~(BIT_TO_MASK(0) | BIT_TO_MASK(16)), PORTC.PDDR);
    
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(31));
    TEST_ASSERT_EQUAL_HEX32(~(BIT_TO_MASK(0) | BIT_TO_MASK(16) | BIT_TO_MASK(31)), PORTC.PDDR);
}

void test_SetPinAsInput_should_NotUpdatePinDirection_when_PinIsNotValid()
{
    PORTC.PDDR = 0xffffffff;

    TEST_ASSERT_EQUAL(GPIO_ERROR, GPIO_SetPinAsInput(32));
    TEST_ASSERT_EQUAL_HEX32(0xffffffff, PORTC.PDDR);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_TheFirst);
    RUN_TEST(test_SetPinAsOutput_should_ConfigurePinDirection);
    RUN_TEST(test_SetPinAsOutput_should_NotUpdatePinDirection_when_PinIsNotValid);
    RUN_TEST(test_SetPinAsInput_should_ConfigurePinDirection);
    RUN_TEST(test_SetPinAsInput_should_NotUpdatePinDirection_when_PinIsNotValid);
    return UNITY_END();
}
