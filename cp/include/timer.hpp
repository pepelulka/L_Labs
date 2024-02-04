#pragma once

#include <chrono>
#include <stdexcept>

namespace timer {

class Timer {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::microseconds;

    Duration dur = Duration(0);
    TimePoint start;
    bool isMeasureStarted = false;
public:
    uint64_t Get();
    void Reset();
    void StartMeasure();
    uint64_t FinishMeasure();
};

}
