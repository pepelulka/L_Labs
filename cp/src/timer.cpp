#include "timer.hpp"

namespace timer {

uint64_t Timer::Get() {
    return dur.count();
}

void Timer::Reset() {
    if (isMeasureStarted) {
        throw std::logic_error("Reseting while measuring.");
    }
    dur = Duration(0);
}

void Timer::StartMeasure() {
    isMeasureStarted = true;
    start = Clock::now();
}

uint64_t Timer::FinishMeasure() {
    if (!isMeasureStarted) {
        throw std::logic_error("Finish measure without start.");
    }
    isMeasureStarted = false;
    TimePoint now = Clock::now();
    Duration dif = std::chrono::duration_cast<Duration>(now - start);
    dur += dif;
    return dif.count();
}

}
