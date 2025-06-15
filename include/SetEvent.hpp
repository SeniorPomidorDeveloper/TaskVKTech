#ifndef SET_EVENT_HPP
#define SET_EVENT_HPP

#include <atomic>

#include "IEvent.hpp"

/**
 * @details A class for tracking metrics without calculating them.
 * 
 * @tparam T the type of metric being collected. It must be thread-safe. 
 * Supports atomic operations.
 * 
 * @startuml
 * class SetEvent <T>
 * {
 *     - __value : T
 *     + setValue(value : T) 
 *     + getValue() : T
 * }
 * @enduml
 */
template<typename T>
class SetEvent final : public IEvent<T>
{
    private:
        std::atomic<T> __value; ///< The metric value

    public:
        SetEvent() = default;
        SetEvent(const SetEvent &other) = delete;
        SetEvent(SetEvent &&other) = delete;

        SetEvent &operator=(const SetEvent &other) = delete;
        SetEvent &operator=(SetEvent &&other) = delete;

        void setValue(const T &value) noexcept;
        T getValue() noexcept override;
};

#include "SetEvent.cpp"

#endif // SET_EVENT_HPP