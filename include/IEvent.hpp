#ifndef I_EVENT_HPP
#define I_EVENT_HPP

/**
 * @brief The event interface
 *
 * @details This class is responsible for updating the metric value. This class
 * is embedded in the stream from which the value enters it.
 * @tparam T the type of metric being collected. It must be thread-safe and have
 * the << operator.
 *
 * @startuml
 * interface IMetric <T>
 * {
 *     + getValue() : T
 * }
 * @enduml
 */
template <typename T>
class IEvent
{
    public:
        virtual ~IEvent() = default;
        virtual T getValue() = 0;
};

#endif // I_EVENT_HPP
