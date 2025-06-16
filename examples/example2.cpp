/**
 * @file example2.cpp
 * @brief Пример использования библиотеки с пользовательскими типами.
 */
#include "Metric.hpp"
#include "MetricManagerBuilder.hpp"
#include "MetricTraits.hpp"
#include "SetEvent.hpp"
#include <atomic>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

// Пользовательский тип
struct NetworkStatus
{
        uint32_t packets_sent; // 4 байта
        uint16_t packets_lost; // 2 байта
        uint16_t bandwidth;    // 2 байта (bandwidth * 100)
};

// Форматирование пользовательского типа
template <>
struct MetricFormatter<NetworkStatus>
{
        static std::string format(const NetworkStatus &value)
        {
            std::ostringstream ss;
            ss << "SENT:" << value.packets_sent
               << ",LOST:" << value.packets_lost << ",BAND:" << std::fixed
               << std::setprecision(2) << (value.bandwidth / 100.0f) << "MBps";
            return ss.str();
        }
};

int main()
{
    try
    {
        // Создаём событие
        auto netEvent = std::make_shared<SetEvent<NetworkStatus>>();

        // Создаём метрику
        auto netMetric =
            std::make_unique<Metric<NetworkStatus>>("network_compact",
                                                    netEvent);

        // Собираем менаджер
        MetricManagerBuilder builder;
        builder.setFile("network_metrics.log")
            .setTimeDelay(std::chrono::seconds(1))
            .addMetric(std::move(netMetric));
        auto manager = builder.build();

        // Запускаем менажер
        manager->run();
        std::atomic<bool> isRunning{true};

        // Формируем поток измерения метрик
        std::thread th{
            [&isRunning, &netEvent]()
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<uint32_t> sentDist(1'000, 10'000);
                std::uniform_int_distribution<uint16_t> lostDist(0, 100);
                std::uniform_int_distribution<uint16_t> bwDist(100, 1'000);
                NetworkStatus status;
                while (isRunning)
                {
                    status = {
                        .packets_sent = sentDist(gen), // Отправленные пакеты
                        .packets_lost = lostDist(gen), // Потерянные пакеты
                        .bandwidth = bwDist(gen)};     // Скорость

                    // Устанавливаем значение
                    netEvent->setValue(status);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
