#include <gtest/gtest.h>

#include "SetEvent.hpp"

TEST(SetEventTest, IntValue)
{
    SetEvent<int> event;
    event.setValue(42);
    EXPECT_EQ(event.getValue(), 42);
}

TEST(SetEventTest, DoubleValue)
{
    SetEvent<double> event;
    event.setValue(3.14);
    EXPECT_DOUBLE_EQ(event.getValue(), 3.14);
}

TEST(SetEventTest, BoolValue)
{
    SetEvent<bool> event;

    event.setValue(true);
    EXPECT_TRUE(event.getValue());

    event.setValue(false);
    EXPECT_FALSE(event.getValue());
}
