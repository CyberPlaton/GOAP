#pragma once

#include <chrono>


class Timer {
public:
    Timer() {}
    ~Timer() {}


    // Start counting time from this point.
    // Initial starting of the timer.
    //
    void startTimer() {
        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }

    /*
    * Elapsed time since "startTimer()" in seconds.
    */
    double getElapsedSeconds() {

        auto endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

        auto duration = end - start;
        return duration / (1000 * 1000); // Duration in secs.
    }


    /*
    * Elapsed time since "startTimer()" in milliseconds.
    */
    double getElapsedMilliseconds() {

        auto endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

        auto duration = end - start;
        return duration / 1000; // Duration in milliseconds
    }


    /*
    * Elapsed time since "startTimer()" in microseconds.
    */
    double getElapsedMicroseconds() {

        auto endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

        auto duration = end - start;
        return duration; // Duration in microsec.
    }


protected:

    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;

private:

};