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

#ifndef SESEMI_MESSAGE_SEMIRT_H_
#define SESEMI_MESSAGE_SEMIRT_H_

#include <memory>
#include <string>

namespace sesemi {

struct SeMIRTWorkerRequest {
  SeMIRTWorkerRequest() = default;
  ~SeMIRTWorkerRequest() = default;

  SeMIRTWorkerRequest(const SeMIRTWorkerRequest& other) = default;
  SeMIRTWorkerRequest(SeMIRTWorkerRequest&& other) = default;
  SeMIRTWorkerRequest& operator=(const SeMIRTWorkerRequest& other) = default;
  SeMIRTWorkerRequest& operator=(SeMIRTWorkerRequest&& other) = default;

  /**
   * @brief encode the request into a string
   */
  std::string EncodeTo() const;

  std::string encrypted_sample_;
  std::string model_name_;
  std::string user_id_;
  std::string key_service_address_;
  uint16_t key_service_port_;
};

SeMIRTWorkerRequest DecodeSeMIRTWorkerRequest(const std::string& request);

struct SeMIRTWorkerResponse {
  std::string EncodeTo() const;

  bool status;
  std::string output;
  std::string aux;
};

SeMIRTWorkerResponse DecodeSeMIRTWorkerResponse(const std::string& response);

struct SeMIRTFnPackerRequest : SeMIRTWorkerRequest {
  std::string EncodeTo() const;

  std::string name;
};

SeMIRTFnPackerRequest DecodeSeMIRTFnPackerRequest(const std::string& request);

struct SeMIRTFnPackerResponse : SeMIRTWorkerResponse {
  std::string EncodeTo() const;
};

SeMIRTFnPackerResponse DecodeSeMIRTFnPackerResponse(const std::string& response);

}  // namespace sesemi

#endif  // SESEMI_MESSAGE_SEMIRT_H_
