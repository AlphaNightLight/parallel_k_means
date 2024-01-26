#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <chrono>
#include <vector>
#include <iostream>

/*
#include <memory>
#include <utility>
#include <algorithm>
#include <random>
#include <type_traits>*/

#include "point.h"



using millis = std::chrono::milliseconds;
using micros = std::chrono::microseconds;
using steady_clock = std::chrono::steady_clock;



template <class T>
struct is_duration : std::false_type{};

template <class R, class P>
struct is_duration<std::chrono::duration<R, P>> : std::true_type{};

template <typename T>
struct measure
{
    static_assert(is_duration<T>::value, "time must be std::chrono::duration");

    template<typename P>
    using ElapsedTimeWithIterations = std::pair<P, int>;

    template <class F, class... Args>
    static ElapsedTimeWithIterations<T> measure_time(F &&func, Args &&... args)
    {
        const auto start = steady_clock::now();
        const auto result = std::forward<F>(func)(std::forward<Args>(args)...);
        const int iteration = 0; // default value for iteration
        return std::make_pair(std::chrono::duration_cast<T>(steady_clock::now() - start), iteration);
    }
};



namespace Utils
{
    std::vector<Observation> readPoints(const std::string path, unsigned int requested_points,
                                        unsigned int requested_dimensions, bool is_centroid = false);

    void writePoints(const std::string path, const std::vector<Observation> &points,
                     const std::vector<Observation> &centroids);
}

#endif // __UTILS_H__
