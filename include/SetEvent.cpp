#include "SetEvent.hpp"

/**
 * @brief The metric value
 * 
 * @tparam T the type of metric being collected. It must be thread-safe. 
 * Supports atomic operations.
 *  
 * @param value the metric value
 */
template<typename T>
void SetEvent<T>::setValue(const T &value) noexcept
{
    __value.store(value);
}

/**
 * @brief The metric value
 * 
 * @tparam T the type of metric being collected. It must be thread-safe. 
 * Supports atomic operations.
 * 
 * @return T - the metric value
 */
template<typename T>
T SetEvent<T>::getValue() noexcept
{
    return __value.load();
}