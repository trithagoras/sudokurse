#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>

class Stopwatch {
public:
    Stopwatch() : start_time(std::chrono::steady_clock::now()) {}
    std::string elapsed_time() const;
    void restart();

private:
    std::chrono::steady_clock::time_point start_time;
};