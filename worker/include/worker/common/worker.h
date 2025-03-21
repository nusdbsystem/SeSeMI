/*
 * Copyright 2024 The SeSeMI Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SESEMIWORKER_COMMON_WORKER_H_
#define SESEMIWORKER_COMMON_WORKER_H_

#include <string>

#include "message/semirt_worker.h"

namespace semirt_worker {

class Worker {
 public:
  Worker() = default;

  virtual ~Worker() = default;

  // delete copy and move constructors and assigment operators
  Worker(const Worker&) = delete;
  Worker& operator=(const Worker&) = delete;
  Worker(Worker&&) = delete;
  Worker& operator=(Worker&&) = delete;

  virtual bool Initialize() = 0;

  virtual bool Handle(uint64_t handle_id, const std::string& sample_request,
                      std::string* output) = 0;

  virtual int Execute(const sesemi::SeMIRTWorkerRequest& request,
                      std::string* output) = 0;

  virtual void Close() = 0;
};

}  // namespace semirt_worker

#endif  // SESEMIWORKER_COMMON_WORKER_H_
