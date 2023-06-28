#include <gtest/gtest.h>

#include "crc.h"

TEST(CrcTests, simple)
{
    uint8_t buf[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint16_t crc = GetCrcModbus(buf, sizeof(buf));
    EXPECT_EQ(53168, crc);
}