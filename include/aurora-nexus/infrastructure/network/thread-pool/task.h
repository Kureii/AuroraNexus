/* This file was created by Kureii (Tomas Adamek)
* Date created: 15. 09. 2023
* This file is under the MIT license
*/
#ifndef AURORANEXUS_TASK_H
#define AURORANEXUS_TASK_H

#include <iostream>
#include <functional>

namespace aurora_nexus {

enum TaskStatus {
  kWait = 0,
  kRunning,
  kFinished,
};

struct Task {
  std::function<void()> job;
  TaskStatus status;

  Task(std::function<void()> job) :job(job), status(kWait) {}

  void execute() {
    status=kRunning;
    job();
    status=kFinished;
  }

};


}

#endif  // AURORANEXUS_TASK_H
