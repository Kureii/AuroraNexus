/* This file was created by Kureii (Tomas Adamek)
* Date created: 15. 09. 2023
* This file is under the MIT license
*/
//================================= Includes ===================================
#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool.hpp"

//================================= Namespace ==================================
namespace aurora_nexus {

//================================= Public method ==============================
ThreadPool::ThreadPool(size_t numThreads) : stop_(false) {
  for (size_t i = 0; i < numThreads; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerFunction, this);
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> const lock(mutex_);
    stop_ = true;
  }

  condition_.notify_all();
  for (std::thread &worker : workers_) {
    worker.join();
  }
}

void ThreadPool::Enqueue(const Task& task) {
  {
    std::unique_lock<std::mutex> const lock(mutex_);
    taskQueue_.push(task);
  }

  condition_.notify_one();
}


//================================= Private method =============================
void ThreadPool::WorkerFunction() {
  while (true) {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this] {
      return stop_ || !taskQueue_.empty();
    });

    if (stop_) {
      return;
    }

    Task task = taskQueue_.front();
    taskQueue_.pop();

    task.execute();
  }
}


//================================= End namespace ==============================
}  // namespace aurora_nexus