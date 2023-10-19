#pragma once

#include "command.h"

class ICommands {
public:
    virtual ~ICommands() = default;

    virtual void HandleCommand(const Command_t& command) = 0;
};
