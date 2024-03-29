/* This file was created by Kureii (Tomas Adamek)
 * Date created: 21. 09. 2023
 * This file is under the MIT license
 */

#ifndef AURORANEXUS_NEXUS_LIST_H
#define AURORANEXUS_NEXUS_LIST_H

#include <memory>
#include <functional>
#include <condition_variable>
#include <atomic>
#include "task.h"

namespace aurora_nexus {

class ThreadPoolNode {
  bool end_node_;

  std::function<void()> job_;
  std::function<void()> on_error_;

  std::shared_ptr<ThreadPoolNode> root_node_;
  std::shared_ptr<ThreadPoolNode> child_node_;
  std::shared_ptr<ThreadPoolNode> next_node_;

  std::atomic_char16_t running_count_;
  std::atomic_uint16_t connected_child_nodes_;

  uint16_t node_limit_;

  std::unique_ptr<Task> task_;

  std::condition_variable condition_;
  mutable std::mutex mutex_;
 public:
  explicit ThreadPoolNode(bool end_node, uint16_t node_limit);
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

  /**
   * @brief Sets the job function to be executed by the thread.
   *
   * This method sets the function that will be executed when the thread
   * is scheduled to run. It should be called before the thread is started.
   *
   * @param new_job The function to set as the job.
   */
  void SetJob(const std::function<void()>& new_job);

  /**
   * @brief Sets the error-handling function to be executed on error.
   *
   * This method sets the function that will be executed in case of an error
   * during the execution of the job. It should be called before the thread is started.
   *
   * @param new_on_error The function to set for error handling.
   */
  void SetOnError(const std::function<void()>& new_on_error);
  void IncrementRunningCount();
  void DecrementRunningCount();
  void IncrementChildNodes();
  void DecrementChildNodes();
};
} // namespace aurora_nexus

#endif  // AURORANEXUS_NEXUS_LIST_H
