#ifndef AURORANEXUS_THREADPOOL_H
#define AURORANEXUS_THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "task.h"
#include "thread-pool-node.h"

namespace aurora_nexus {

class ThreadPool {
 public:
  ThreadPool(uint32_t initial_count, uint16_t step_size, uint64_t queue_size);
  ~ThreadPool();
  void Enqueue(const Task& task);

#ifdef ENABLE_TESTS
  [[nodiscard]] size_t GetThreadCount() const;
  [[nodiscard]] std::shared_ptr<ThreadPoolNode> GetRootNode() const;
  // size_t GetRunnungTasks();
#endif
  void MapStructure(std::map<std::shared_ptr<ThreadPoolNode>, bool>& map,
                    std::shared_ptr<ThreadPoolNode> root_node);

 private:
  uint32_t initial_count_;
  uint16_t step_size_;
  std::atomic_int64_t queue_size_;
  uint64_t thread_count_;
  std::shared_ptr<ThreadPoolNode> root_node_;
  mutable std::mutex mutex_;

  void IncrementThreadCount();
  void DecrementThreadCount();
};

}  // namespace aurora_nexus

#endif  // AURORANEXUS_THREADPOOL_H