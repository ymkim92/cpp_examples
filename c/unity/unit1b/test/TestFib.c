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
    TEST_ASSERT_EQUAL_INT(2, Fibonacci_GetElement(0));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_element0_should_return1);
    return UNITY_END();
}
