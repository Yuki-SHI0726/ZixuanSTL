// SimpleInstrumentationProfiler.h
#pragma once

#ifndef MANUAL_PROFILER

#include "HighPrecisionTimer.h"
#include <string>
#include <iostream>

class SimpleInstrumentationProfiler
{
	HighPrecisionTimer m_timer;
	std::string m_label;

public:
	SimpleInstrumentationProfiler(const char* label)
		: m_label(label)
	{
		m_timer.StartTimer();
	}

    SimpleInstrumentationProfiler(const SimpleInstrumentationProfiler&) = delete;
    SimpleInstrumentationProfiler(SimpleInstrumentationProfiler&&) = delete;
    SimpleInstrumentationProfiler& operator=(const SimpleInstrumentationProfiler&) = delete;
    SimpleInstrumentationProfiler& operator=(SimpleInstrumentationProfiler&&) = delete;

	~SimpleInstrumentationProfiler()
	{
		const double result = m_timer.GetTimer();
		std::cout << "Result for " << m_label << ": " << result << std::endl;
	}
};

#define START_PROFILER(_str_) SimpleInstrumentationProfiler profiler(_str_)

#else  // !MANUAL_PROFILER, so we need to disable this simer

#define START_PROFILER(_str_)

#endif  // MANUAL_PROFILER
