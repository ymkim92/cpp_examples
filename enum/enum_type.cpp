#include <iostream>

enum class StatusReg {
    BUSY,
    REF_OVERFLOW,
};

enum class RegLLV3 : std::uint8_t {
    ACQ_CMD = 0x00,
    STATUS = 0x01,
    SIG_CNT_VAL = 0x02,
    ACQ_CONFIG = 0x04,
};

int main()
{
    std::uint8_t ui8Value {};

    // ui8Value = RegLLV3::STATUS;
    // ui8Value = StatusReg::BUSY;

    std::cout << RegLLV3::STATUS << std::endl;
    return 0;
}