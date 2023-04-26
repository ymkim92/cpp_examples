#include <stdio.h>
#include <stdint.h>

typedef struct 
{
    uint8_t m_reserved: 6;
    uint8_t m_dirty: 1;     // bit1
    uint8_t m_release: 1;   // LSB bit0
} FlagField_t;

typedef union {
    uint8_t m_data;
    FlagField_t m_fields;
} VersionFlag_t;

int main()
{
    VersionFlag_t flag;
    flag.m_data = 2;
    printf("release: %d, dirty: %d\n", flag.m_fields.m_release, flag.m_fields.m_dirty);
    return 0;
}