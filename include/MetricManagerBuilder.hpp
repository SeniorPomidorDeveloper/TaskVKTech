#ifndef METRIC_MANAGER_BUILDER_HPP
#define METRIC_MANAGER_BUILDER_HPP

#include <chrono>

#include "MetricManager.hpp"

/**
 * @brief A class that creates an object of the MetricManager class
 * 
 * @startuml
 * class MetricManagerBuilder
 * {
 *     # __filePath : std::string
 *     # __timeDelay : std::chrono::milliseconds
 *     # __metrics : IMetric[*]
 *     # validate()
 *     + setFile(filePath : std::string) : MetricManagerBuilder
 *     + setTimeDelay(timeDelay : std::chrono::milliseconds) : MetricManagerBuilder
 *     + addMetric(metricPtr : IMetric) : MetricManagerBuilder
 * }
 * @enduml
 */
class MetricManagerBuilder final
{
    protected:
        std::string __filePath; ///< The path to the recording file.
        std::chrono::milliseconds __timeDelay{1'000};   ///< The time interval for checking metrics.
        std::vector<std::unique_ptr<IMetric>> __metrics; ///< A vector of metrics.

        void validate();

    public: 
        MetricManagerBuilder() = default;
        MetricManagerBuilder(const MetricManagerBuilder &other) = delete;
        MetricManagerBuilder(MetricManagerBuilder &&other) = delete;

        MetricManagerBuilder &operator=(const MetricManagerBuilder &other) =
            delete;
        MetricManagerBuilder &operator=(MetricManagerBuilder &&other) = delete;

        ~MetricManagerBuilder() = default;

        MetricManagerBuilder &setFile(const std::string &filePath);
        MetricManagerBuilder &setTimeDelay(
            const std::chrono::milliseconds &timeDelay);
        MetricManagerBuilder &addMetric(std::unique_ptr<IMetric> &&metricPtr);

        std::unique_ptr<MetricManager> build();
};

#endif // METRIC_MANAGER_BUILDER_HPP
