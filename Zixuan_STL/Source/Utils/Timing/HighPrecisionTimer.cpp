// HighPrecisionTimer.cpp
#include "HighPrecisionTimer.h"

using namespace std::chrono;

void HighPrecisionTimer::StartTimer()
{
    m_startTime = high_resolution_clock::now();
}

double HighPrecisionTimer::GetTimer() const
{
    const TimePoint endTime = high_resolution_clock::now();
    TimeDuration timeSpan = duration_cast<TimeDuration>(endTime - m_startTime);
    return timeSpan.count() * 1000;  // convert to milliseconds before returning
}
