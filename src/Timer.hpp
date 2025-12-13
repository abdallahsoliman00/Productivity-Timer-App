/* Timer.hpp */
#pragma once

#include "raylib.h"
#include <string>
#include <sstream>
#include <iomanip>


class Timer {
private:
    int SetTime_m;
    double StartTime_s;
    double SetTime_s;
    double PausedTime_s = 0.0;

public:
    bool isRunning = false;

    Timer() : SetTime_m(20), StartTime_s(0.0), SetTime_s(1200.0) {}


    void SetTimer(const int time_m) {
        SetTime_m = time_m;
        SetTime_s = time_m * 60.0;
    }

    void Pause() {
        if (isRunning) {
            PausedTime_s = ElapsedTime();
            isRunning = false;
        }
    }

    void Resume() {
        if (!isRunning) {
            StartTime_s = GetTime() - PausedTime_s;
            isRunning = true;
        }
    }

    void Reset() {
        isRunning = false;
        PausedTime_s = 0.0;
    }

    [[nodiscard]] double ElapsedTime() const {
        if (isRunning) {
            return GetTime() - StartTime_s;
        }
        return PausedTime_s;
    }

    [[nodiscard]] double TimeRemaining() const {
        return SetTime_s - ElapsedTime();
    }

    [[nodiscard]] float Progress() const {
        if (SetTime_s > 0.0)
            return ElapsedTime() / SetTime_s;
        return 0.0f;
    }

    [[nodiscard]] bool Complete() const {
        return ElapsedTime() >= SetTime_s;
    }

    [[nodiscard]] std::string GetCountdownTime() const {
        if (!isRunning && PausedTime_s == 0) {
            return std::to_string(SetTime_m) + ":00";
        }
        const double tr = TimeRemaining();
        if (tr <= 0) return "00:00";

        const int mins = std::floor(tr / 60.0);
        const int secs = static_cast<int>(tr) % 60;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << mins
            << ":"
            << std::setw(2) << std::setfill('0') << secs;

        return oss.str();
    }

};
