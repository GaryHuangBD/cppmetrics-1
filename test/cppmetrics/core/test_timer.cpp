/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_timer.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: vpoliboy
 */

#include "cppmetrics/core/timer.h"
#include <chrono>
#include <gtest/gtest.h>
#include <random>
#include <thread>

namespace cppmetrics {
namespace core {

TEST(timer, initialTest)
{
    Timer timer;
    ASSERT_EQ((uint64_t)0, timer.getCount());
    ASSERT_NEAR(0.0, timer.getMeanRate(), 0.000001);
    ASSERT_NEAR(0.0, timer.getOneMinuteRate(), 0.000001);
    ASSERT_NEAR(0.0, timer.getFiveMinuteRate(), 0.000001);
    ASSERT_NEAR(0.0, timer.getFifteenMinuteRate(), 0.000001);

    timer.update(std::chrono::seconds(1));
    ASSERT_EQ((uint64_t)1, timer.getCount());
}

TEST(timer, timerContextTest)
{
    Timer timer;
    std::mt19937 rng;
    for (size_t i = 0; i < 100; ++i) {
        std::uniform_int_distribution<> uniform(10, 30);
        size_t sleep_time = uniform(rng);
        TimerContextPtr time_context(timer.timerContextPtr());
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
    ASSERT_EQ((uint64_t)100, timer.getCount());
    SnapshotPtr snapshot = timer.getSnapshot();
    // On jenkins builds, when there is lot of load, the duration of the sleep
    // in the timerContextTest takes more than the 20 ns. This is to eliminate
    // the periodic test failures during CI.

    std::cout << "             count = " << timer.getCount() << std::endl;
    std::cout << "         mean rate = " << (timer.getMeanRate())
              << " calls per 1 sec" << std::endl;
    std::cout << "     1-minute rate = " << (timer.getOneMinuteRate())
              << " calls per 1 sec" << std::endl;
    std::cout << "     5-minute rate = " << (timer.getFiveMinuteRate())
              << " calls per 1 sec" << std::endl;
    std::cout << "    15-minute rate = " << (timer.getFifteenMinuteRate())
              << " calls per 1 sec" << std::endl;
    std::cout << "               min = " << (snapshot->getMin()) << " ns "
              << std::endl;
    std::cout << "               max = " << (snapshot->getMax()) << " ns "
              << std::endl;
    std::cout << "              mean = " << (snapshot->getMean()) << " ns "
              << std::endl;
    std::cout << "            stddev = " << (snapshot->getStdDev()) << " ns "
              << std::endl;
    std::cout << "            median = " << (snapshot->getMedian()) << " ns "
              << std::endl;
    std::cout << "              75% <= " << (snapshot->get75thPercentile())
              << " ns " << std::endl;
    std::cout << "              95% <= " << (snapshot->get95thPercentile())
              << " ns " << std::endl;
    std::cout << "              98% <= " << (snapshot->get98thPercentile())
              << " ns " << std::endl;
    std::cout << "              99% <= " << (snapshot->get99thPercentile())
              << " ns " << std::endl;
    std::cout << "            99.9% <= " << (snapshot->get999thPercentile())
              << " ns " << std::endl;
    ASSERT_LE(25, static_cast<int>(timer.getMeanRate()));
    ASSERT_GE(55, static_cast<int>(timer.getMeanRate()));
    ASSERT_LE(std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::milliseconds(8))
                  .count(),
        static_cast<int>(snapshot->getMin()));
    ASSERT_GE(std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::milliseconds(45))
                  .count(),
        static_cast<int>(snapshot->getMax()));
}
}
}
