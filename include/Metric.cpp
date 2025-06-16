#include "Metric.hpp"

#include "MetricTraits.hpp"

/**
 * @brief Constructor of the Metric<T> class
 *
 * @tparam T the type of metric being collected. It must be thread-safe and have
 * the << operator. Supports atomic operations.
 * @param name the name of the metric.
 * @param event A pointer to the event being monitored.
 */
template <typename T>
Metric<T>::Metric(const std::string_view &name,
                  std::shared_ptr<IEvent<T>> event) noexcept
    : __name{name},
      __eventPtr{event}
{
}

/**
 * @brief Name Getter
 *
 * @tparam T the type of metric being collected. It must be thread-safe and have
 * the << operator. Supports atomic operations.
 *
 * @return std::string_view - the name of the metric.
 */
template <typename T>
std::string_view Metric<T>::getName() const
{
    return __name;
}

/**
 * @brief Upload the data.
 *
 * @tparam T the type of metric being collected. It must be thread-safe and have
 * the << operator. Supports atomic operations.
 *
 * @return std::string - the metric value converted to a string type.
 */
template <typename T>
std::string Metric<T>::fetch()
{
    return MetricFormatter<T>::format(__eventPtr->getValue());
}
