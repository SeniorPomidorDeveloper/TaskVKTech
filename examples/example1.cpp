/**
 * @file example1.cpp
 * @brief Пример использования библиотеки со стандартными типами.
 */
#include "Metric.hpp"
#include "MetricManagerBuilder.hpp"
#include "SetEvent.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

int main()
{
    try
    {
        // Создаём события
        auto counterEvent = std::make_shared<SetEvent<int>>();
        auto tempEvent = std::make_shared<SetEvent<double>>();
        auto memoryEvent = std::make_shared<SetEvent<double>>();

        // Создаём метрики
        auto counterMetric =
            std::make_unique<Metric<int>>("requests_count", counterEvent);
        auto tempMetric =
            std::make_unique<Metric<double>>("cpu_temp", tempEvent);
        auto memoryMetric =
            std::make_unique<Metric<double>>("memory_usage", memoryEvent);

        // Собираем менажер
        MetricManagerBuilder builder;
        builder.setFile("metrics.log")
            .setTimeDelay(std::chrono::seconds(1))
            .addMetric(std::move(counterMetric))
            .addMetric(std::move(tempMetric))
            .addMetric(std::move(memoryMetric));
        auto manager = builder.build();

        // Запускаем менажер
        manager->run();
        std::atomic<bool> isRunning{true};

        // Формируем поток измерения метрик
        std::thread th{
            [&tempEvent, &counterEvent, &isRunning, &memoryEvent]()
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> countDist(
                    0,
                    1'000); // Счетчик запросов
                std::normal_distribution<double> tempDist(
                    45.0,
                    10.0); // Температура CPU (нормальное распределение)
                std::exponential_distribution<double> memoryDist(
                    0.2); // Счётчик памяти
                while (isRunning)
                {
                    // Генерируем случайные значения
                    int requests = countDist(gen);
                    double temp = std::clamp(tempDist(gen),
                                             20.0,
                                             90.0); // Ограничиваем диапазон
                    double memory =
                        std::clamp(memoryDist(gen),
                                   0.0,
                                   5.0); // Использование памяти в ГБ

                    // Устанавливаем значения
                    counterEvent->setValue(requests);
                    tempEvent->setValue(temp);
                    memoryEvent->setValue(memory);

                    // Случайная задержка между обновлениями (200-800 мс)
                    std::uniform_int_distribution<int> delayDist(200, 800);
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(delayDist(gen)));
                }
            }};

        while (isRunning)
        {
            std::string command;
            std::cin >> command;
            if (command == "stop")
            {
                isRunning.store(false);
                break;
            }
        }
        th.join();

        manager->stop();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
