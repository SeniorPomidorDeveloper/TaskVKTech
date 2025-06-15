#include "MetricManagerBuilder.hpp"

/**
 * @brief Checking the set values
 * 
 * @throw std::invalid_argument - If the file path is empty or the time interval
 *  is not allowed.
 */
void MetricManagerBuilder::validate()
{
    if (__filePath.empty())
    {
        throw std::invalid_argument("An empty file path is passed!");
    }
    if (__timeDelay.count() <= 0)
    {
        throw std::invalid_argument("An invalid time interval is transmitted!");
    }
}

/**
 * @brief Set the path to the recording file
 * 
 * @param filePath the path to the recording file.
 * @return this. 
 */
MetricManagerBuilder &MetricManagerBuilder::setFile(const std::string &filePath)
{
    __filePath = filePath;
    return *this;
}

/**
 * @brief Set the time interval between recordings.
 * 
 * @param timeDelay the time interval.
 * @return this
 */
MetricManagerBuilder &MetricManagerBuilder::setTimeDelay(
    const std::chrono::milliseconds &timeDelay)
{
    __timeDelay = timeDelay;
    return *this;
}

/**
 * @brief Add a metric.
 * 
 * @param metricPtr - a pointer to the metric.
 * @return MetricManagerBuilder& 
 */
MetricManagerBuilder &MetricManagerBuilder::addMetric(
    std::unique_ptr<IMetric> &&metricPtr)
{
    __metrics.push_back(std::move(metricPtr));
    return *this;
}

/**
 * @brief Create a metrics manager.
 * 
 * @return pointer of metrics manager
 */
std::unique_ptr<MetricManager> MetricManagerBuilder::build()
{
    validate();
    std::unique_ptr<MetricManager> mManager{new MetricManager()};
    mManager->__filePath = std::move(__filePath);
    mManager->__timeDelay = __timeDelay;
    std::for_each(__metrics.begin(),
                  __metrics.end(),
                  [&mManager](auto &metric)
                  {
                      mManager->addMetric(std::move(metric));
                  });
    return mManager;
}
