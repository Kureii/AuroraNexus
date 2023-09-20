#ifndef AURORANEXUS_THREADPOOL_H
#define AURORANEXUS_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include "task.h"

namespace aurora_nexus {

class ThreadPool {
 public:
  ThreadPool(size_t numThreads, size_t maxQueueSize, uint16_t dynamicStepSize);
  ~ThreadPool();
  void Enqueue(const Task& task);

#ifdef ENABLE_TESTS
  size_t GetWorkerCount();
#endif

 private:
  void WorkerFunction();
  void AddWorkers();
  void RemoveWorkers();
  void WaitAndRemoveWorker(std::thread::id worker_id);
  void DynamicScalingFunction();
  size_t CountRunningThreads();

  size_t initial_threads_num_;
  std::vector<std::thread> workers_;
  std::queue<Task> task_queue_;
  std::thread scaling_thread_;
  size_t max_queue_size_;
  uint16_t dynamic_step_size_;
  std::map<std::thread::id, TaskStatus> thread_status_map_;

  std::mutex mutex_;
  std::condition_variable condition_;
  bool stop_;
  bool dynamic_scaling_enabled_;
};

}  // namespace aurora_nexus

#endif  // AURORANEXUS_THREADPOOL_H