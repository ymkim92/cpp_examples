#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t m_timestamp;
    uint32_t m_command;
    bool m_connected;
} Command_t;
