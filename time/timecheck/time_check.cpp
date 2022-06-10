#include "time_check.h"
// #include "spdlog/spdlog.h"

#include <iostream>
#include <sstream>
#include <thread>

using namespace std::chrono;

static constexpr uint8_t MAX_NUMBER_OF_OVERTIME_DISPLAYED { 1 };

TimeCheck::TimeCheck(const std::string message, std::chrono::milliseconds interval_ms)
    : m_message { message }
    , m_interval_ms { interval_ms }
    , m_prev_time {}
    , m_vector_lack_of_time_us {}
    , m_cnt_overtime {}
{
}

void TimeCheck::PrintLogIfNowIsLaterThanTargetTime(const time_point<steady_clock>& time_target)
{
    auto time_now = steady_clock::now();
    std::ostringstream ss;

    if (time_now >= time_target) {
        m_vector_lack_of_time_us.push_back(duration_cast<microseconds>(time_now - time_target).count());
    }

    if (m_vector_lack_of_time_us.size() && (duration_cast<milliseconds>(time_now - m_prev_time) > m_interval_ms)) {
        for (uint32_t item : m_vector_lack_of_time_us) {
            m_cnt_overtime += 1;
            if (MAX_NUMBER_OF_OVERTIME_DISPLAYED < m_cnt_overtime) {
                ss << "... ";
                break;
            }
            ss << item << " ";
        }
        // spdlog::warn("{} # of cases [{}]: {} (us) for last {} ms", m_message, m_vector_lack_of_time_us.size(), ss.str(), m_interval_ms.count());
        std::cout << m_message << " # of cases [" << m_vector_lack_of_time_us.size() << "]: " << ss.str() << " (us) for last " << m_interval_ms.count() << " ms" << std::endl;

        m_vector_lack_of_time_us.clear();
        m_cnt_overtime = 0;
        m_prev_time = time_now;
    }
}

TimeCheck::~TimeCheck(void) = default;

int main()
{
    std::array<uint32_t, 10> delay_time = {49, 69, 79, 89, 49, 49, 49, 49, 49, 51};
    std::shared_ptr<TimeCheck> time_check = std::make_shared<TimeCheck>("TEST!!!", milliseconds(100));
    for (int i=0; i<10; i++)
    {
        auto target_time = steady_clock::now();
        target_time += milliseconds(50);
        std::this_thread::sleep_for(milliseconds(delay_time[i]));
        time_check->PrintLogIfNowIsLaterThanTargetTime(target_time);
        std::this_thread::sleep_until(target_time);
    }
    return 0;
}