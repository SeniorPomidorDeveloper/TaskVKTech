#include "IEvent.hpp"
#include "Metric.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>

template <typename T>
class MockEvent : public IEvent<T>
{
    public:
        MOCK_METHOD(T, getValue, (), (override));
};

TEST(MetricTest, IntMetricBasic)
{
    auto event = std::make_shared<MockEvent<int>>();
    Metric<int> metric("requests", event);

    EXPECT_EQ(metric.getName(), "requests");
}

TEST(MetricTest, IntMetricFetch)
{
    auto event = std::make_shared<MockEvent<int>>();
    EXPECT_CALL(*event, getValue()).WillOnce(testing::Return(42));

    Metric<int> metric("requests", event);
    EXPECT_EQ(metric.fetch(), "42");
}

TEST(MetricTest, DoubleMetricBasic)
{
    auto event = std::make_shared<MockEvent<double>>();
    Metric<double> metric("temperature", event);

    EXPECT_EQ(metric.getName(), "temperature");
}

TEST(MetricTest, DoubleMetricFetch)
{
    auto event = std::make_shared<MockEvent<double>>();
    EXPECT_CALL(*event, getValue()).WillOnce(testing::Return(36.6));

    Metric<double> metric("temperature", event);
    EXPECT_EQ(metric.fetch(), "36.60");
}

TEST(MetricTest, BoolMetricBasic)
{
    auto event = std::make_shared<MockEvent<bool>>();
    Metric<bool> metric("status", event);

    EXPECT_EQ(metric.getName(), "status");
}

TEST(MetricTest, BoolMetricFetchTrue)
{
    auto event = std::make_shared<MockEvent<bool>>();
    EXPECT_CALL(*event, getValue()).WillOnce(testing::Return(true));

    Metric<bool> metric("status", event);
    EXPECT_EQ(metric.fetch(), "1");
}

TEST(MetricTest, BoolMetricFetchFalse)
{
    auto event = std::make_shared<MockEvent<bool>>();
    EXPECT_CALL(*event, getValue()).WillOnce(testing::Return(false));

    Metric<bool> metric("status", event);
    EXPECT_EQ(metric.fetch(), "0");
}

TEST(MetricTest, MultipleFetchCalls)
{
    auto event = std::make_shared<MockEvent<int>>();
    Metric<int> metric("counter", event);

    EXPECT_CALL(*event, getValue())
        .Times(3)
        .WillOnce(testing::Return(10))
        .WillOnce(testing::Return(20))
        .WillOnce(testing::Return(30));

    EXPECT_EQ(metric.fetch(), "10");
    EXPECT_EQ(metric.fetch(), "20");
    EXPECT_EQ(metric.fetch(), "30");
}

TEST(MetricTest, DifferentEventsSameMetric)
{
    auto event1 = std::make_shared<MockEvent<int>>();
    auto event2 = std::make_shared<MockEvent<int>>();

    Metric<int> metric1("metric1", event1);
    Metric<int> metric2("metric1", event2);

    EXPECT_CALL(*event1, getValue()).WillOnce(testing::Return(100));
    EXPECT_CALL(*event2, getValue()).WillOnce(testing::Return(200));

    EXPECT_EQ(metric1.fetch(), "100");
    EXPECT_EQ(metric2.fetch(), "200");
}
