#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;

void PrintLog(const std::string& message, const time_point<steady_clock>& time_target, const milliseconds& interval_ms)
{
    auto time_now = steady_clock::now();
    static time_point<steady_clock> prev {};
    static std::vector<uint32_t> vecLackOfTimeMs;
    std::stringstream ss;

    if (time_now >= time_target) {
        vecLackOfTimeMs.push_back(duration_cast<microseconds>(time_now - time_target).count()/100);
    }

    if (vecLackOfTimeMs.size() && (duration_cast<milliseconds>(time_now - prev) > interval_ms)) {
        std::cout << vecLackOfTimeMs.size() << std::endl;
        for (uint32_t item: vecLackOfTimeMs) {
            ss << item << " ";
        }
        std::cout << ss.str() << "(100us) for last " << interval_ms.count() << " ms"<<std::endl;
        vecLackOfTimeMs.clear();

        prev = time_now;
    }
    
}

int main()
{
    std::array<uint32_t, 10> delay_time = {49, 69, 79, 89, 49, 49, 49, 49, 49, 51};
    for (int i=0; i<10; i++)
    {
        auto target_time = steady_clock::now();
        target_time += milliseconds(50);
        std::this_thread::sleep_for(milliseconds(delay_time[i]));
        PrintLog("TEST", target_time, milliseconds(100));
        std::this_thread::sleep_until(target_time);
    }
    return 0;
}