#include "MetricManager.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>

/**
 * @brief Move Constructor
 *
 * @param other the object being moved.
 */
MetricManager::MetricManager(MetricManager &&other)
{
    *this = std::move(other);
}

MetricManager &MetricManager::operator=(MetricManager &&other)
{
    std::swap(__metrics, other.__metrics);
    std::swap(__filePath, other.__filePath);
    __isRunning.store(other.__isRunning);
    __timeDelay = other.__timeDelay;
    std::swap(__workingCycle, other.__workingCycle);
    return *this;
}

/**
 * @brief The destructor of the metrics manager.
 *
 * Stops writing to a file.
 */
MetricManager::~MetricManager()
{
    try
    {
        stop();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * @brief Adds a new metric to write to the file.
 *
 * @param metricPtr A pointer to the metric to add.
 *
 * @throw std::invalid_argument - If the manager is already running.
 */
void MetricManager::addMetric(std::unique_ptr<IMetric> &&metricPtr)
{
    if (!metricPtr)
    {
        throw std::invalid_argument("A nullptr is passed!");
    }
    __metrics.push_back(std::move(metricPtr));
}

/**
 * @brief Adds a new entry to the file.
 *
 * @param out the stream that the data will be written to.
 */
void MetricManager::newEntry(std::ostream &out)
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch() % std::chrono::seconds(1));

    out << std::put_time(std::localtime(&time), "%F %T.%")
        << milliseconds.count();
    std::for_each(__metrics.begin(),
                  __metrics.end(),
                  [&out](auto &metricPtr)
                  {
                      out << ' ' << metricPtr->getName() << ' '
                          << metricPtr->fetch();
                  });
    out << std::endl;

    out.flush();
}

/**
 * @brief Starts the process of writing metrics to a file.
 *
 * @throw std::runtime_error - If the file could not be opened.
 */
void MetricManager::run()
{
    __workingCycle =
        std::thread{[this]()
                    {
                        std::ofstream file(__filePath, std::ios::app);
                        if (!file.is_open())
                        {
                            throw std::runtime_error("Couldn't open the file!");
                        }
                        __isRunning.store(true);
                        while (__isRunning.load())
                        {
                            newEntry(file);
                            std::this_thread::sleep_for(__timeDelay);
                        }
                        file.close();
                    }};
}

/**
 * @brief Stops the process of writing metrics to a file.
 */
void MetricManager::stop()
{
    __isRunning.store(false);
    if (__workingCycle.joinable())
    {
        __workingCycle.join();
    }
}
