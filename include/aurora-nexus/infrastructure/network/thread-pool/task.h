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
  TaskStatus status;

  Task() : status(kWait) {};

  void execute(const std::function<void()>& job, const std::function<void()>& on_error) {
    status = kRunning;
    if (on_error == nullptr) {
        std::cerr << "Warning: on_error in task "<< this << " is nullptr.\n";
    }
    if (job == nullptr) {
        std::cerr << "Warning: on_error in task " << this << " is nullptr.\n";
    }
    try {
      job();
    } catch (...) {
      if (on_error != nullptr) {
        on_error();
      }
    }
    status = kFinished;
  }
};

}  // namespace aurora_nexus

#endif  // AURORANEXUS_TASK_H
