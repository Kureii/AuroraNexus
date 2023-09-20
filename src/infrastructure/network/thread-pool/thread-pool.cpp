/* This file was created by Kureii (Tomas Adamek)
 * Date created: 15. 09. 2023
 * This file is under the MIT license
 */
//================================= Includes ===================================
#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool.hpp"

#include <algorithm>
#include <chrono>

#define DYNAMIC_MULTIPLAYER 8 / 10
//================================= Namespace ==================================
namespace aurora_nexus {

//================================= Public method ==============================
ThreadPool::ThreadPool(size_t numThreads, size_t maxQueueSize,
                       uint16_t dynamicStepSize)
    : stop_(false),
      dynamic_scaling_enabled_(true),
      max_queue_size_(maxQueueSize),
      dynamic_step_size_(dynamicStepSize),
      initial_threads_num_(numThreads)
{
  for (size_t i = 0; i < numThreads; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerFunction, this);
  }
  scaling_thread_ = std::thread(&ThreadPool::DynamicScalingFunction, this);
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> const lock(mutex_);
    stop_ = true;
  }

  condition_.notify_all();  // probudí všechna čekající vlákna
  dynamic_scaling_enabled_ = false;

  if (scaling_thread_.joinable()) {
    scaling_thread_.detach();
  }

  {
    for (std::thread &worker : workers_) {
      if (worker.joinable()) {
        worker.detach();
      }
    }
  }
}

void ThreadPool::Enqueue(const Task& task) {
  {
    std::unique_lock<std::mutex> const lock(mutex_);
    if (task_queue_.size() >= max_queue_size_) {
      return;
    }
    task_queue_.push(task);
  }

  condition_.notify_one();
}

//================================= Testing method =============================

#ifdef ENABLE_TESTS
size_t ThreadPool::GetWorkerCount() { return workers_.size(); }
#endif

//================================= Private method =============================
void ThreadPool::WorkerFunction() {
  while (true) {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this] { return stop_ || !task_queue_.empty(); });

    if (stop_) {
      return;
    }

    Task task = task_queue_.front();
    task_queue_.pop();

    auto threadId = std::this_thread::get_id();
    thread_status_map_[threadId] = kRunning;
    lock.unlock();
    task.execute();
    lock.lock();
    thread_status_map_[threadId] = kFinished;
  }
}

void ThreadPool::AddWorkers() {
  std::unique_lock<std::mutex> const lock(mutex_);
  for (size_t i = 0; i < dynamic_step_size_; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerFunction, this);
  }
}

void ThreadPool::RemoveWorkers() {
  // TODO: implement remove workers, inactive or detach
  /*std::unique_lock<std::mutex> const lock(mutex_);

  auto to_remove = dynamic_step_size_;
  auto it = workers_.rbegin();

  while (to_remove > 0 && it != workers_.rend()) {
    auto worker_id = it->get_id();

    if (thread_status_map_[worker_id] != kRunning) {
      // v tomto případě bys potřeboval také zastavit a joinovat vlákno,
      // jinak bude zdrojový únik
      it->detach();
      workers_.erase((++it).base());  // konverze z reverse_iterator na iterator
      thread_status_map_.erase(worker_id);
      --to_remove;
    } else {
      std::thread watcher(&ThreadPool::WaitAndRemoveWorker, this, worker_id);
      watcher.detach();
      --to_remove;
    }
    ++it;
  }*/
}

void ThreadPool::WaitAndRemoveWorker(std::thread::id worker_id) {
  // Čekáme, až bude vlákno v stavu kFinished.
  while (true) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));  // čekáme malou chvíli
    if (thread_status_map_[worker_id] == kFinished) {
      std::unique_lock<std::mutex> const lock(mutex_);
      // Najdeme vlákno a provedeme join.
      auto it = std::find_if(workers_.begin(), workers_.end(),
                             [worker_id](const std::thread& t) {
                               return t.get_id() == worker_id;
                             });
      if (it != workers_.end()) {
        it->join();
        workers_.erase(it);
      }
      thread_status_map_.erase(worker_id);
      return;
    }
  }
}

void ThreadPool::DynamicScalingFunction() {
  while (dynamic_scaling_enabled_) {
    auto thread_count = CountRunningThreads();
    if (workers_.size() * DYNAMIC_MULTIPLAYER <= thread_count) {
      if (workers_.size() + dynamic_step_size_ < max_queue_size_) {
        AddWorkers();
      }
    } else if (workers_.size() - dynamic_step_size_ * DYNAMIC_MULTIPLAYER >
               thread_count && workers_.size() != initial_threads_num_) {
      RemoveWorkers();
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds (50));
    }
  }
}

size_t ThreadPool::CountRunningThreads() {
  return std::count_if(
      thread_status_map_.begin(), thread_status_map_.end(),
      [](const auto& pair) { return pair.second == kRunning; });
}

//================================= End namespace ==============================
}  // namespace aurora_nexus