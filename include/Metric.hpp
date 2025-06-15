#ifndef METRIC_HPP
#define METRIC_HPP

#include <memory>

#include "IEvent.hpp"
#include "IMetric.hpp"

/**
 * @details A template metric is an observer in relation to an event.
 *
 * @tparam T the type of metric being collected. It must be thread-safe and have
 * the << operator.
 *
 * @startuml
 * class Metric <T> extends IMetric
 * {
 *     - __name : std::string
 *     - __eventPtr : IEvent<T>
 *     + getName() : std::string_view
 *     + fetch() : std::string
 * }
 * @enduml
 */
template <typename T>
class Metric final : public IMetric
{
    private:
        std::string_view __name; ///< Name of the metric
        std::shared_ptr<IEvent<T>>
            __eventPtr; ///< A pointer to the observed event

    public:
        Metric(const std::string_view &name,
               std::shared_ptr<IEvent<T>> event) noexcept;
        Metric(const Metric &other) = delete;
        Metric(Metric &&other) = delete;

        Metric &operator=(const Metric &other) = delete;
        Metric &operator=(Metric &other) = delete;

        std::string_view getName() const override;
        std::string fetch() override;
};

#include "Metric.cpp"

#endif // METRIC_HPP
