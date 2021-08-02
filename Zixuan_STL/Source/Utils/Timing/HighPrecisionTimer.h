// HighPrecisionTimer.h
#pragma once

#include <chrono>

class HighPrecisionTimer
{
    typedef std::chrono::high_resolution_clock::time_point TimePoint;
    typedef std::chrono::duration<double> TimeDuration;

    TimePoint m_startTime;

public:
    void StartTimer();
    double GetTimer() const;
};
