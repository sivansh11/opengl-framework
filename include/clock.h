#ifndef HORIZON_CLOCK_H
#define HORIZON_CLOCK_H

#include "core.h"

typedef std::chrono::_V2::system_clock::time_point time_t_;

class HorizonClock
{
public:
    HorizonClock()
    {
        from();
    }

    template <typename T>
    T till()
    {
        time_t_ newTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<T, std::chrono::seconds::period>(newTime - m_time).count();
    }
    template <typename T>
    T frameTime()
    {
        time_t_ newTime = std::chrono::high_resolution_clock::now();
        T timeDuration = std::chrono::duration<T, std::chrono::seconds::period>(newTime - m_time).count();
        m_time = newTime;
        return timeDuration;
    }
    void from()
    {
        m_time = (time_t_)std::chrono::high_resolution_clock::now();
    }

    void sleep(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

private:
    time_t_ m_time;
};



#endif