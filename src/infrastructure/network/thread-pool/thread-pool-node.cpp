/* This file was created by Kureii (Tomas Adamek)
* Date created: 15. 09. 2023
* This file is under the MIT license
*/
//================================= Includes ===================================
#include "aurora-nexus/infrastructure/network/thread-pool/thread-pool-node.h"

//================================= Namespace ==================================
namespace aurora_nexus {

//================================= Public method ==============================

ThreadPoolNode::ThreadPoolNode(bool end_node)
    : end_node_(end_node)
      {
  task_ = std::make_unique<Task>(Task(nullptr, nullptr));
}

void ThreadPoolNode::ThreadLoop() {
  while(end_node_) {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this] {return !end_node_ || task_->status == kWait; });

    if (!end_node_) {
      break;
    }

    task_->execute();

    if (root_node_ != nullptr) {
      // TODO: counter
    }
  }

}

bool ThreadPoolNode::IsEndNode() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return end_node_;
}

uint16_t ThreadPoolNode::GetRunningCount() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return running_count_;
}

const std::shared_ptr<ThreadPoolNode> &ThreadPoolNode::GetRootNode() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return root_node_;
}

const std::shared_ptr<ThreadPoolNode> &ThreadPoolNode::GetChildNode() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return child_node_;
}

const std::shared_ptr<ThreadPoolNode> &ThreadPoolNode::GetNextNode() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return next_node_;
}

TaskStatus ThreadPoolNode::GetTaskStatus() const {
  std::lock_guard<std::mutex> const lock(mutex_);
  return task_->status;
}

void ThreadPoolNode::SetRootNode(
    const std::shared_ptr<ThreadPoolNode> &rootNode) {
  std::lock_guard<std::mutex> const lock(mutex_);
  root_node_ = rootNode;
}

void ThreadPoolNode::SetChildNode(
    const std::shared_ptr<ThreadPoolNode> &childNode) {
  std::lock_guard<std::mutex> const lock(mutex_);
  child_node_ = childNode;
}

void ThreadPoolNode::SetNextNode(
    const std::shared_ptr<ThreadPoolNode> &nextNode) {
  std::lock_guard<std::mutex> const lock(mutex_);
  next_node_ = nextNode;
}

void ThreadPoolNode::IncrementRunningCount() {
  std::lock_guard<std::mutex> const lock(mutex_);
  ++running_count_;
}

void ThreadPoolNode::DecrementRunningCount() {
  std::lock_guard<std::mutex> const lock(mutex_);
  --running_count_;
}
void ThreadPoolNode::IncrementChildNodes() {
  std::lock_guard<std::mutex> const lock(mutex_);
  ++connected_child_nodes_;
}
void ThreadPoolNode::DecrementChildNodes() {
  std::lock_guard<std::mutex> const lock(mutex_);
  --connected_child_nodes_;
}
uint16_t ThreadPoolNode::GetConnectedChildNodes() const {
  return connected_child_nodes_;
}
//================================= Testing method =============================


//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace aurora_nexus