#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <thread>

#include "Metric.hpp"
#include "MetricManagerBuilder.hpp"
#include "SetEvent.hpp"

namespace fs = std::filesystem;

class MockMetric : public IMetric
{
    public:
        MockMetric(std::string name, std::string value)
            : name_(std::move(name)),
              value_(std::move(value))
        {
        }

        std::string_view getName() const override
        {
            return name_;
        }

        std::string fetch() override
        {
            return value_;
        }

    private:
        std::string name_;
        std::string value_;
};

class MetricManagerTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            temp_file = fs::temp_directory_path() / "test_metrics.txt";
        }

        void TearDown() override
        {
            if (fs::exists(temp_file))
            {
                fs::remove(temp_file);
            }
        }

        fs::path temp_file;
};

TEST_F(MetricManagerTest, BuilderThrowsWhenFilePathEmpty)
{
    MetricManagerBuilder builder;
    builder.setTimeDelay(std::chrono::milliseconds(100));
    EXPECT_THROW(builder.build(), std::invalid_argument);
}

TEST_F(MetricManagerTest, BuilderThrowsWhenTimeDelayInvalid)
{
    MetricManagerBuilder builder;
    builder.setFile("test.txt");
    builder.setTimeDelay(std::chrono::milliseconds(0));
    EXPECT_THROW(builder.build(), std::invalid_argument);
}

TEST_F(MetricManagerTest, BuilderCreatesManagerSuccessfully)
{
    MetricManagerBuilder builder;
    builder.setFile(temp_file.string())
        .setTimeDelay(std::chrono::milliseconds(100));

    auto metric = std::make_unique<MockMetric>("test", "value");
    builder.addMetric(std::move(metric));

    auto manager = builder.build();
    EXPECT_NE(manager, nullptr);
}

TEST_F(MetricManagerTest, WritesMetricsToFileCorrectly)
{
    MetricManagerBuilder builder;
    builder.setFile(temp_file.string())
        .setTimeDelay(std::chrono::milliseconds(1));

    auto mock_metric = std::make_unique<MockMetric>("mock", "42");
    builder.addMetric(std::move(mock_metric));

    auto event = std::make_shared<SetEvent<int>>();
    event->setValue(100);
    auto int_metric = std::make_unique<Metric<int>>("int_metric", event);
    builder.addMetric(std::move(int_metric));

    auto manager = builder.build();
    manager->run();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    manager->stop();

    std::ifstream in(temp_file);
    ASSERT_TRUE(in.is_open());

    std::string line;
    std::getline(in, line);
    EXPECT_NE(line.find("mock 42"), std::string::npos);
    EXPECT_NE(line.find("int_metric 100"), std::string::npos);
}

TEST_F(MetricManagerTest, WritesMultipleEntries)
{
    MetricManagerBuilder builder;
    builder.setFile(temp_file.string())
        .setTimeDelay(std::chrono::milliseconds(1));

    auto metric = std::make_unique<MockMetric>("counter", "1");
    builder.addMetric(std::move(metric));

    auto manager = builder.build();
    manager->run();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    manager->stop();

    std::ifstream in(temp_file);
    int line_count = 0;
    std::string line;
    while (std::getline(in, line))
    {
        line_count++;
    }

    EXPECT_GE(line_count, 5)
        << "Expected at least 5 entries, got " << line_count;
}

TEST_F(MetricManagerTest, StopAutomaticallyInDestructor)
{
    {
        MetricManagerBuilder builder;
        builder.setFile(temp_file.string())
            .setTimeDelay(std::chrono::milliseconds(1));

        auto event = std::make_shared<SetEvent<int>>();
        event->setValue(123);
        auto metric = std::make_unique<Metric<int>>("temp_metric", event);
        builder.addMetric(std::move(metric));

        auto manager = builder.build();
        manager->run();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    std::ifstream in(temp_file);
    std::string line;
    std::getline(in, line);
    EXPECT_FALSE(line.empty());
    EXPECT_NE(line.find("temp_metric 123"), std::string::npos);
}

TEST_F(MetricManagerTest, HandlesMultipleMetrics)
{
    MetricManagerBuilder builder;
    builder.setFile(temp_file.string())
        .setTimeDelay(std::chrono::milliseconds(1));

    builder.addMetric(std::make_unique<MockMetric>("cpu", "75%"));
    builder.addMetric(std::make_unique<MockMetric>("mem", "1.2GB"));
    builder.addMetric(std::make_unique<MockMetric>("net", "100Mbps"));

    auto manager = builder.build();
    manager->run();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    manager->stop();

    std::ifstream in(temp_file);
    std::string line;
    std::getline(in, line);

    EXPECT_NE(line.find("cpu 75%"), std::string::npos);
    EXPECT_NE(line.find("mem 1.2GB"), std::string::npos);
    EXPECT_NE(line.find("net 100Mbps"), std::string::npos);
}
