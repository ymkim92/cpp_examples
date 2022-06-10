#ifndef __TIME_CHECK_H__
#define __TIME_CHECK_H__

#include <chrono>
#include <string>
#include <vector>

class TimeCheck {
public:
    TimeCheck(const std::string message, std::chrono::milliseconds interval_ms);
    ~TimeCheck(void);
    void PrintLogIfNowIsLaterThanTargetTime(const std::chrono::time_point<std::chrono::steady_clock>& time_target);

private:
    std::string m_message;
    std::chrono::milliseconds m_interval_ms;
    std::chrono::time_point<std::chrono::steady_clock> m_prev_time;
    std::vector<uint32_t> m_vector_lack_of_time_us;
    uint8_t m_cnt_overtime;
};

#endif // __TIME_CHECK_H__