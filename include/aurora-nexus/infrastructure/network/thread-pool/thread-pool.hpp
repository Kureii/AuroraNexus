#ifndef AURORANEXUS_THREADPOOL_H
#define AURORANEXUS_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include "task.h"
#include "thread-pool-node.h"

namespace aurora_nexus {

class ThreadPool {
 public:
  ThreadPool(uint32_t initial_count, uint16_t step_size, uint64_t queue_size);
  ~ThreadPool();
  void Enqueue(const Task& task);

//#ifdef ENABLE_TESTS
  [[nodiscard]]size_t GetThreadCount() const;
  //size_t GetRunnungTasks();
//#endif

 private:
  uint32_t initial_count_;
  uint16_t step_size_;
  uint64_t queue_size_;
  uint64_t thread_count_;
  std::shared_ptr<ThreadPoolNode> root_node_;
  mutable std::mutex mutex_;

  void IncrementThreadCount();
  void DecrementThreadCount();

};

}  // namespace aurora_nexus

#endif  // AURORANEXUS_THREADPOOL_H