/* This file was created by Kureii (Tomas Adamek)
* Date created: 20. 09. 2023
* This file is under the MIT license
*/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

TEST(THREAD_POOL, Initialize) {
  auto *tp = new aurora_nexus::ThreadPool(10, 100, 5);
  ASSERT_EQ(tp->GetWorkerCount(), 10);
  delete tp;
}

TEST(THREAD_POOL, InitializeWithDynamicScalingFunction) {
  auto *tp = new aurora_nexus::ThreadPool(10, 100, 5);
  std::this_thread::sleep_for(std::chrono::milliseconds (200));
  ASSERT_EQ(tp->GetWorkerCount(), 10);
  delete tp;
}

TEST(THREAD_POOL, TestDynamicScaling) {

  aurora_nexus::ThreadPool tp(10, 100, 5);

  ASSERT_EQ(tp.GetWorkerCount(), 10);

  auto myLambda = [](){std::this_thread::sleep_for(std::chrono::seconds(15));};
  aurora_nexus::Task const myTask(myLambda);

  for (int i = 0; i < 10; ++i) {
    tp.Enqueue(myTask);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds (500));

  ASSERT_EQ(tp.GetWorkerCount(), 15);
}

