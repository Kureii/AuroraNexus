/* This file was created by Kureii (Tomas Adamek)
 * Date created: 20. 09. 2023
 * This file is under the MIT license
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <exception>

#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool.hpp"

#define NUM_THREADS 300
#define NUM_QUEUE 1000
#define NUM_STEP 50
#define UP_STEP_MULTIPLIER 0.95
#define DOWN_STEP_MULTIPLIER 0.5

TEST(THREAD_POOL, initialize_throw_initial_count_is_not_multipl_step_size) {
  ASSERT_THROW(aurora_nexus::ThreadPool(40,50,1000), std::underflow_error);
  try {
    auto thread_pool = aurora_nexus::ThreadPool(40,50,1000);
    FAIL();
  } catch (std::underflow_error& e) {
    ASSERT_STREQ("\"initial_count\" must be a multiple of \"step_size\"\n", e.what());
  } catch (...) {
    FAIL();
  }
}

TEST(THREAD_POOL, initialize_throw_queue_size_is_not_multipl_step_size) {
  ASSERT_THROW(aurora_nexus::ThreadPool(50,50,830), std::underflow_error);
  try {
    auto thread_pool = aurora_nexus::ThreadPool(50,50,830);
    FAIL();
  } catch (std::underflow_error& e) {
    ASSERT_STREQ("\"queue_size\" must be bigger than sum of \"initial_count\" and \"step_size\".\n\"queue_size\" must be a multiple of \"step_size\"\n", e.what());
  } catch (...) {
    FAIL();
  }
}

TEST(THREAD_POOL, initialize_throw_queue_size_is_small) {
  ASSERT_THROW(aurora_nexus::ThreadPool(50,50,50), std::underflow_error);
  try {
    auto thread_pool = aurora_nexus::ThreadPool(50,50,50);
    FAIL();
  } catch (std::underflow_error& e) {
    ASSERT_STREQ("\"queue_size\" must be bigger than sum of \"initial_count\" and \"step_size\".\n\"queue_size\" must be a multiple of \"step_size\"\n", e.what());
  } catch (...) {
    FAIL();
  }
}

TEST(THREAD_POOL, initialize_warning_queue_size_is_small) {
  std::stringstream const buffer;
  auto *oldCerrStreamBuf = std::cerr.rdbuf();
  std::cerr.rdbuf(buffer.rdbuf());

  auto thread_pool = aurora_nexus::ThreadPool(50,50,100);

  std::cerr.rdbuf(oldCerrStreamBuf);

  ASSERT_EQ(buffer.str(), "Warning: \"queue_size\" is as large as the sum of \"initial_count\" and \"step_size\". This usage reduces efficiency.\n"); // očekává konkrétní výpis
}

TEST(THREAD_POOL, initialize_warning_step_size_is_small) {
  std::stringstream const buffer;
  auto *oldCerrStreamBuf = std::cerr.rdbuf();
  std::cerr.rdbuf(buffer.rdbuf());

  auto thread_pool = aurora_nexus::ThreadPool(50,5,100);

  std::cerr.rdbuf(oldCerrStreamBuf);

  ASSERT_EQ(buffer.str(), "Warning: \"step_size\" is too small, this usage reduces efficiency.\n"); // očekává konkrétní výpis
}

//void addTask(aurora_nexus::ThreadPool& tp, size_t seconds_for_thread,
//             size_t count_add_threads) {
//  auto myLambda = [&seconds_for_thread]() {
//    std::this_thread::sleep_for(std::chrono::seconds(seconds_for_thread));
//  };
//  aurora_nexus::Task const myTask(myLambda);
//
//  for (size_t i = 0; i < count_add_threads; ++i) {
//    tp.Enqueue(myTask);
//  }
//}
//
TEST(THREAD_POOL, Initialize_minimum_threads) {
  auto tp = std::make_unique<aurora_nexus::ThreadPool>(50,50,1000);

  auto result = 50;

  ASSERT_EQ(tp->GetThreadCount(), result);
}

TEST(THREAD_POOL, Initialize_more_than_minimum_threads) {
  auto tp = std::make_unique<aurora_nexus::ThreadPool>(30,10,1000);

  auto result = 30;

  ASSERT_EQ(tp->GetThreadCount(), result);
}
//
//TEST(THREAD_POOL, InitializeWithDynamicScalingFunction) {
//  {
//    auto tp = std::make_unique<aurora_nexus::ThreadPool>(
//        NUM_THREADS, NUM_QUEUE, NUM_STEP, UP_STEP_MULTIPLIER,
//        DOWN_STEP_MULTIPLIER);
//
//    auto result = NUM_THREADS;
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//    ASSERT_EQ(tp->GetWorkerCount(), result);
//  }
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//}
//
//TEST(THREAD_POOL, TestDynamicGrowing) {
//  {
//    auto tp = std::make_unique<aurora_nexus::ThreadPool>(
//        NUM_THREADS, NUM_QUEUE, NUM_STEP, UP_STEP_MULTIPLIER,
//        DOWN_STEP_MULTIPLIER);
//
//    auto result = NUM_THREADS;
//
//    ASSERT_EQ(tp->GetWorkerCount(), result);
//
//    addTask(*tp, 5, NUM_THREADS);
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(750));
//
//    result = NUM_THREADS + NUM_STEP;
//
//    ASSERT_EQ(tp->GetWorkerCount(), result);
//  }
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//}
//
//TEST(THREAD_POOL, TestDynamicScaling) {
//  {
//    auto tp = std::make_unique<aurora_nexus::ThreadPool>(
//        NUM_THREADS, NUM_QUEUE, NUM_STEP, UP_STEP_MULTIPLIER,
//        DOWN_STEP_MULTIPLIER);
//
//    auto result = NUM_THREADS;
//
//    ASSERT_EQ(tp->GetWorkerCount(), result);
//
//    addTask(*tp, 2, NUM_THREADS);
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(750));
//
//    result = NUM_THREADS + NUM_STEP;
//
//    ASSERT_EQ(tp->GetWorkerCount(), result);
//
//    result = NUM_THREADS;
//
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    std::cout << "running tasks: " << tp->GetRunnungTasks() << std::endl;
//
//    ASSERT_EQ(tp->GetWorkerCount(), result);
//  }
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//}