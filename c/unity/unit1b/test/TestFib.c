#include <limits.h>
#include "unity.h"
#include "Fib.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_element0_should_return1(void)
{
    TEST_ASSERT_EQUAL_INT(1, Fibonacci_GetElement(0));
}

void test_element1_should_return1(void)
{
    TEST_ASSERT_EQUAL_INT(1, Fibonacci_GetElement(1));
}

void test_element2_should_return2(void)
{
    TEST_ASSERT_EQUAL_INT(2, Fibonacci_GetElement(2));
}

void test_part_of_sequence(void)
{
    int expected[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144};
    int i;
    for (i=0; i<sizeof(expected)/sizeof(expected[0]); i++)
        TEST_ASSERT_EQUAL_INT(expected[i], Fibonacci_GetElement(i));
}

void test_negative_number_returns0()
{
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(-1));
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(-555));
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(INT_MIN));
}

void test_value1_should_return1()
{
    TEST_ASSERT_EQUAL_INT(1, Fibonacci_IsInSequence(1));
}

void test_value3_should_return1()
{
    TEST_ASSERT_EQUAL_INT(1, Fibonacci_IsInSequence(3));
}

void test_value20_should_return0()
{
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_IsInSequence(20));
}

void test_value_negative_should_return0()
{
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_IsInSequence(-1));
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_IsInSequence(INT_MIN));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_element0_should_return1);
    RUN_TEST(test_element1_should_return1);
    RUN_TEST(test_element2_should_return2);
    RUN_TEST(test_part_of_sequence);
    RUN_TEST(test_negative_number_returns0);
    
    RUN_TEST(test_value1_should_return1);
    RUN_TEST(test_value3_should_return1);
    RUN_TEST(test_value20_should_return0);
    RUN_TEST(test_value_negative_should_return0);
    return UNITY_END();
}
