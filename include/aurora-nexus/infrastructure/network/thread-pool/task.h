/* This file was created by Kureii (Tomas Adamek)
 * Date created: 15. 09. 2023
 * This file is under the MIT license
 */
#ifndef AURORANEXUS_TASK_H
#define AURORANEXUS_TASK_H

#include <functional>
#include <iostream>

namespace aurora_nexus {

enum TaskStatus {
  kWait = 0,
  kRunning,
  kFinished,
};

struct Task {
  std::function<void()> job_;
  std::function<void()> on_error_;
  TaskStatus status;

  Task(std::function<void()> job, std::function<void()> on_error)
      : job_(job), on_error_(on_error), status(kWait) {}

  void execute() {
    status = kRunning;
    try {
      job_();
    } catch (...) {
      on_error_();
    }
    status = kFinished;
  }
};

}  // namespace aurora_nexus

#endif  // AURORANEXUS_TASK_H
