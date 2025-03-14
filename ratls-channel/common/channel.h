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

#ifndef SESEMI_RATLSCHANNEL_COMMON_CHANNEL_H_
#define SESEMI_RATLSCHANNEL_COMMON_CHANNEL_H_

#define RCVBUFSIZE 4096

#include <memory>
#include <string>

namespace ratls {

struct CheckContext {};

class Channel {
 public:
  virtual ~Channel() = default;
  virtual int CloseConnection() = 0;
  virtual int Read(std::string* output) = 0;
  virtual int Send(const std::string& msg) = 0;
};
}  // namespace ratls

#endif  // SESEMI_RATLSCHANNEL_COMMON_CHANNEL_H_
