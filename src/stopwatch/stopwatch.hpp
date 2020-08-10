#include <chrono>


struct Stopwatch {
    Stopwatch(std::chrono::nanoseconds& result);
    ~Stopwatch();
    private:
        std::chrono::nanoseconds& result;
        const std::chrono::time_point<std::chrono::high_resolution_clock> start;
};