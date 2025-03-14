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

#ifndef SESEMIWORKER_INFERENCERT_TVMCRT_COMMON_ABSTRACT_H_
#define SESEMIWORKER_INFERENCERT_TVMCRT_COMMON_ABSTRACT_H_

#ifdef __cplusplus
extern "C" {
#endif

void tvm_default_print_string(const char* str);

void tvm_default_print_time();

void tvm_crt_exit(int error_code);

#ifdef __cplusplus
}
#endif

#endif  // SESEMIWORKER_INFERENCERT_TVMCRT_COMMON_ABSTRACT_H_
