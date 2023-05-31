#include "unity.h"
#include "Fib.h"

void setUp(void)
{
}

void tearDown(void)
{
}
void test_TheFirst(void)
{
    TEST_IGNORE_MESSAGE("Implement Me");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_TheFirst);
    return UNITY_END();
}
