/* This file was created by Kureii (Tomas Adamek)
 * Date created: 21. 09. 2023
 * This file is under the MIT license
 */

#ifndef AURORANEXUS_NEXUS_LIST_H
#define AURORANEXUS_NEXUS_LIST_H

#include <memory>
#include <functional>
#include <condition_variable>
#include "task.h"

namespace aurora_nexus {

class ThreadPoolNode {
  bool end_node_ = false;
  std::shared_ptr<ThreadPoolNode> root_node_;
  std::shared_ptr<ThreadPoolNode> child_node_;
  std::shared_ptr<ThreadPoolNode> next_node_;
  std::condition_variable condition_;
  mutable std::mutex mutex_;
  uint16_t running_count_;
  uint16_t connected_child_nodes_;
  std::unique_ptr<Task> task_;
 public:
  explicit ThreadPoolNode(bool end_node);
  void ThreadLoop();

  [[nodiscard]] uint16_t GetRunningCount() const;
  [[nodiscard]] uint16_t GetConnectedChildNodes() const;
  [[nodiscard]] const std::shared_ptr<ThreadPoolNode>& GetRootNode() const;
  [[nodiscard]] const std::shared_ptr<ThreadPoolNode>& GetChildNode() const;
  [[nodiscard]] const std::shared_ptr<ThreadPoolNode>& GetNextNode() const;
  [[nodiscard]] TaskStatus GetTaskStatus() const;
  [[nodiscard]] bool IsEndNode() const;

  void SetRootNode(const std::shared_ptr<ThreadPoolNode>& rootNode);
  void SetChildNode(const std::shared_ptr<ThreadPoolNode>& childNode);
  void SetNextNode(const std::shared_ptr<ThreadPoolNode>& nextNode);
  void IncrementRunningCount();
  void DecrementRunningCount();
  void IncrementChildNodes();
  void DecrementChildNodes();
};
} // namespace aurora_nexus

#endif  // AURORANEXUS_NEXUS_LIST_H
