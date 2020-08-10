#include <chrono>
#include "stopwatch.hpp"

Stopwatch::Stopwatch(std::chrono::nanoseconds& result) : result{ result }, start{ std::chrono::high_resolution_clock::now() } { }

Stopwatch::~Stopwatch() {
    result = std::chrono::high_resolution_clock::now() - start;
}
