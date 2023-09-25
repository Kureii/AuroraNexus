/* This file was created by Kureii (Tomas Adamek)
 * Date created: 15. 09. 2023
 * This file is under the MIT license
 */
//================================= Includes ===================================
#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool.hpp"

#include <algorithm>
#include <boost/math/special_functions/round.hpp>
#include <chrono>
#include <cmath>
#include <exception>
#include <list>

#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool-node.h"

//================================= Namespace ==================================
namespace aurora_nexus {

//================================= Public method ==============================
ThreadPool::ThreadPool(uint32_t initial_count, uint16_t step_size,
                       uint64_t queue_size) {
  if (initial_count % step_size == 0) {
    initial_count_ = initial_count;
    step_size_ = step_size;
  } else {
    throw std::underflow_error(
        "\"initial_count\" must be a multiple of \"step_size\"\n");
  }

  if (queue_size % step_size != 0 || queue_size < initial_count + step_size) {
    throw std::underflow_error(
        "\"queue_size\" must be bigger than sum of \"initial_count\" and "
        "\"step_size\".\n\"queue_size\" must be a multiple of \"step_size\"\n");
  }
  if (queue_size == initial_count + step_size) {
    std::cerr << "Warning: \"queue_size\" is as large as the sum of "
                 "\"initial_count\" and \"step_size\". This usage reduces "
                 "efficiency.\n";
  }
  if (step_size <= 5) {
    std::cerr << "Warning: \"step_size\" is too small, this usage reduces "
                 "efficiency.\n";
  }

  thread_count_ = 0;
  if (initial_count == step_size) {
    // Initialize non endpoint as a root node
    root_node_ = std::make_shared<ThreadPoolNode>(false, step_size);

    // Initialize endpoint as a child of root node
    auto last_node = std::make_shared<ThreadPoolNode>(true, step_size);
    last_node->SetRootNode(root_node_);
    root_node_->SetChildNode(last_node);
    IncrementThreadCount();

    // Add other endpoints
    for (uint16_t i = 0; i < initial_count - 1; ++i) {
      auto shared_tmp = std::make_shared<ThreadPoolNode>(true, step_size);
      shared_tmp->SetRootNode(root_node_);
      last_node->SetNextNode(shared_tmp);
      last_node = shared_tmp;
      IncrementThreadCount();
    }
  } else {
    // Create all end points lists and save to end_points_lists
    auto end_points_lists = new std::list<std::shared_ptr<ThreadPoolNode>>;
    for (uint32_t i = 0; i < initial_count;) {
      auto last_node = std::make_shared<ThreadPoolNode>(true, step_size);
      IncrementThreadCount();
      end_points_lists->emplace_back(last_node);
      for (uint16_t j = 0; j < step_size - 1; ++j) {
        auto shared_tmp = std::make_shared<ThreadPoolNode>(true, step_size);
        last_node->SetNextNode(shared_tmp);
        last_node = shared_tmp;
        IncrementThreadCount();
      }
      i += step_size;
    }

    auto non_end_points_lists = end_points_lists;

    while (non_end_points_lists->size() != 1) {
      auto non_end_points_lists_tmp =
          new std::list<std::shared_ptr<ThreadPoolNode>>;

      while (!non_end_points_lists->empty()) {
        auto mini_root_node = std::make_shared<ThreadPoolNode>(false, step_size);
        auto last_node = non_end_points_lists->back();
        non_end_points_lists->pop_back();
        {
          auto tmp_last_node = last_node;
          while (tmp_last_node) {
            tmp_last_node->SetRootNode(mini_root_node);
            tmp_last_node->GetRootNode()->IncrementChildNodes();
            tmp_last_node = tmp_last_node->GetNextNode();
          }
        }
        mini_root_node->SetChildNode(last_node);
        non_end_points_lists_tmp->emplace_back(mini_root_node);
        // printNode(mini_root_node);

        auto mini_root_node_next = mini_root_node;
        for (uint16_t j = 0;
             j < step_size - 1 && !non_end_points_lists->empty(); ++j) {
          mini_root_node_next = std::make_shared<ThreadPoolNode>(false, step_size);
          mini_root_node->SetNextNode(mini_root_node_next);
          last_node = non_end_points_lists->back();
          mini_root_node_next->SetChildNode(last_node);
          non_end_points_lists->pop_back();
          {
            auto tmp_last_node = last_node;
            while (tmp_last_node) {
              tmp_last_node->SetRootNode(mini_root_node_next);
              tmp_last_node->GetRootNode()->IncrementChildNodes();
              tmp_last_node = tmp_last_node->GetNextNode();
            }
          }
          mini_root_node = mini_root_node_next;
        }
      }
      non_end_points_lists = non_end_points_lists_tmp;
    }
    root_node_ = non_end_points_lists->back();
    while (root_node_->GetRootNode()) {
      root_node_ = root_node_->GetRootNode();
    }
    if (root_node_->GetNextNode() != nullptr &&
        root_node_->GetNextNode()->GetConnectedChildNodes() >
            root_node_->GetConnectedChildNodes()) {
      auto last = root_node_->GetNextNode();
      auto first = last;
      while (last->GetNextNode() != nullptr) {
        last = last->GetNextNode();
      }
      last->SetNextNode(root_node_);
      root_node_->SetNextNode(nullptr);
      root_node_ = first;
    }
  }
}

ThreadPool::~ThreadPool() { root_node_ = nullptr; }

void ThreadPool::Enqueue(const Task& task) {
  //  {
  //    std::unique_lock<std::mutex> const lock(mutex_);
  //    if (task_queue_.size() >= max_queue_size_) {
  //      return;
  //    }
  //    task_queue_.push(task);
  //  }
  //
  //  worker_condition_.notify_one();
}

//================================= Testing method =============================

#ifdef ENABLE_TESTS
size_t ThreadPool::GetThreadCount() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return thread_count_;
}
std::shared_ptr<ThreadPoolNode> ThreadPool::GetRootNode() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return root_node_;
}
// size_t ThreadPool::GetRunnungTasks() {return std::count_if(
//           thread_status_map_.begin(), thread_status_map_.end(),
//           [](const auto& pair) { return pair.second == kRunning; });}
#endif

//================================= Private method =============================

void ThreadPool::IncrementThreadCount() {
  std::lock_guard<std::mutex> const lock(mutex_);
  ++thread_count_;
}

void ThreadPool::DecrementThreadCount() {
  std::lock_guard<std::mutex> const lock(mutex_);
  --thread_count_;
}

//================================= End namespace ==============================

}  // namespace aurora_nexus