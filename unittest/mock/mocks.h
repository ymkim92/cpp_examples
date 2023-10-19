#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "interface.h"

using ::testing::NiceMock;
using ::testing::StrictMock;

class MockCommands : public ICommands {
public:
    MOCK_METHOD1(HandleCommand, void(const Command_t&));
};
typedef StrictMock<MockCommands> StrictMockCommands;
typedef NiceMock<MockCommands> NiceMockCommands;