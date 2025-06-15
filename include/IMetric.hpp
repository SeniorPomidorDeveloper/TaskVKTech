#ifndef I_METRIC_HPP
#define I_METRIC_HPP

#include <string>

/**
 * @brief Interface of the metrica class. 
 * 
 * @details This interface is responsible for collecting the 
 * value from the source and converting it to the desired format for correct 
 * display.
 * 
 * @startuml
 * interface IMetric
 * {
 *     + getName() : std::string
 *     + fetch() : std::string
 * }
 * @enduml
 */
class IMetric
{
    public:
        virtual ~IMetric() = default;
        virtual std::string_view getName() const = 0;    ///< Метод получения названия метрики
        virtual std::string fetch() = 0;    ///< Получение значения
};

#endif // I_METRIC_HPP
