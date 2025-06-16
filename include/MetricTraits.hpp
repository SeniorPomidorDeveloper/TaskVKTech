#ifndef METRIC_TRAITS_HPP
#define METRIC_TRAITS_HPP

#include <iomanip>
#include <sstream>
#include <type_traits>

template <typename T>
struct AtomicSupport    ///< Checking that the type supports atomic operations.
{
        static constexpr bool value =
            std::is_trivially_copyable_v<T> &&
            std::is_copy_constructible_v<T> &&
            std::is_move_constructible_v<T> && std::is_copy_assignable_v<T> &&
            std::is_move_assignable_v<T>;
};

template <typename T>
struct MetricTypeValidator  ///< Checking that the type is suitable for creating a metric.
{
        static constexpr bool is_valid =
            AtomicSupport<T>::value && std::is_default_constructible_v<T>; 
};

template <typename T>
struct MetricFormatter  ///< Formatting the type into a string for the metric.
{
        static std::string format(const T &value)
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(2) << value;
                return ss.str();
            }
            else
            {
                return std::to_string(value);
            }
        }
};

#endif // METRIC_TRAITS_HPP
