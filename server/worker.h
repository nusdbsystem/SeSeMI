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

#ifndef SESEMI_SERVER_WORKER_H_
#define SESEMI_SERVER_WORKER_H_

#include <string>

namespace sesemi {

class ServiceWorker {
 public:
  ServiceWorker() = default;

  virtual ~ServiceWorker() = default;

  // delete copy and move constructors and assigment operators
  ServiceWorker(const ServiceWorker&) = delete;
  ServiceWorker& operator=(const ServiceWorker&) = delete;
  ServiceWorker(ServiceWorker&&) = delete;
  ServiceWorker& operator=(ServiceWorker&&) = delete;

  virtual bool Initialize() = 0;

  virtual bool Handle(const std::string& url, const std::string& input,
                      std::string* output) = 0;

  virtual void Close() = 0;
};

}  // namespace sesemi

#endif  // SESEMI_SERVER_WORKER_H_