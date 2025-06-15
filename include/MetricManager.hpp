#ifndef METRIC_MANAGER_HPP
#define METRIC_MANAGER_HPP

#include <chrono>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <filesystem>

#include "IMetric.hpp"

class MetricManagerBuilder;

/**
 * @details A class for tracking metrics without calculating them.
 * 
 * @tparam T the type of metric being collected. It must be thread-safe. 
 * Supports atomic operations.
 * 
 * @startuml
 * class MetricManager
 * {
 *     # __metrics : IMetric[*]
 *     # __filePath : std::filesystem::path
 *     # __timeDelay : const std::chrono::milliseconds
 *     # __isRunning : bool
 *     # __workingCycle : std::thread
 *     # addMetric(metricPtr : IMetric)
 *     # newEntry(out : std::ostream)
 *     + run()
 *     + stop()
 * }
 * @enduml
 */
class MetricManager
{
    friend MetricManagerBuilder;

    protected:
        std::vector<std::unique_ptr<IMetric>> __metrics;    ///< A vector of pointers to metrics.
        std::filesystem::path __filePath;   ///< The path to the file with the saved metrics data.
        std::chrono::milliseconds __timeDelay;    ///< The frequency with which the metrics need to be checked.
        std::atomic<bool> __isRunning;  ///< The manager's job status.
        std::thread __workingCycle; ///< The work flow of the manager's record.

        MetricManager() = default;

        void addMetric(std::unique_ptr<IMetric> &&metricPtr);
        void newEntry(std::ostream &out);

    public:
        MetricManager(const MetricManager &other) = delete;
        MetricManager(MetricManager &&other);

        MetricManager &operator=(const MetricManager &other) = delete;
        MetricManager &operator=(MetricManager &&other);

        ~MetricManager();

        void run();
        void stop();
};

#endif // METRIC_MANAGER_HPP
