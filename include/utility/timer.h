#pragma once
#include <chrono>
#include <iostream>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> _start{std::chrono::steady_clock::now()};

public:
    void reset() {
        _start = std::chrono::steady_clock::now();
    }

    [[nodiscard]] double elapsed() const {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - _start).count();
    }
};

inline std::ostream &operator<<(std::ostream &os, const Timer &timer) {
    os << "Timer: {" << timer.elapsed() * 1000.0f << "} ms";
    return os;
}
