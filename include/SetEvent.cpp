#include "SetEvent.hpp"

/**
 * @brief The metric value
 *
 * @tparam T the type of metric being collected. It must be thread-safe.
 * Supports atomic operations.
 *
 * @param value the metric value
 */
template <typename T>
void SetEvent<T>::setValue(const T &value)
{
    __value.store(value);
    if (!__value.is_lock_free())
    {
        throw std::runtime_error(
            "Atomic operations for type '" + std::string(typeid(T).name()) +
            "' require locks. This may cause performance issues.");
    }
}

/**
 * @brief The metric value
 *
 * @tparam T the type of metric being collected. It must be thread-safe.
 * Supports atomic operations.
 *
 * @return T - the metric value
 */
template <typename T>
T SetEvent<T>::getValue() noexcept
{
    return __value.exchange(T());
}
