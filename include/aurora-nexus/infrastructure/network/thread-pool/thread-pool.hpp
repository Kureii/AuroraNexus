#ifndef AURORANEXUS_THREADPOOL_H
#define AURORANEXUS_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "task.h"

namespace aurora_nexus {

class ThreadPool {
 public:
  ThreadPool(size_t numThreads);
  ~ThreadPool();
  void Enqueue(const Task& task);

 private:
  void WorkerFunction();

  std::vector<std::thread> workers_;
  std::queue<Task> taskQueue_;

  std::mutex mutex_;
  std::condition_variable condition_;
  bool stop_;
};

}  // namespace aurora_nexus

#endif  // AURORANEXUS_THREADPOOL_H