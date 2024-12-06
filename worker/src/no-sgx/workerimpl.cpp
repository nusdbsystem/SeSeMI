#include "worker/no-sgx/workerimpl.h"

#include <cassert>
#include <iostream>
#include <mutex>

#include "worker/common/config.h"
#include "worker/inference-runtime/model/model.h"
#include "message/semirt_worker.h"
#include "utils/hexutil.h"

namespace semirt_worker {
namespace {
// #ifndef NDEBUG
std::mutex debug_log_mutex;
// #endif // NDEBUG

// // model is stored at the untrusted side
thread_local std::string model_rt_id_;
thread_local ModelRT* model_rt_;

void ClearExecutionContext() {
  if (model_rt_) {
    free_model_rt(model_rt_);
    model_rt_ = nullptr;
  }
  model_rt_id_.clear();
}
}  // anonymous namespace

bool WorkerImpl::Initialize() {
  initialize_ = true;
  return true;
}

bool WorkerImpl::Handle(uint64_t handle_id, const std::string& sample_request,
                        std::string* output) {
  auto msg_prefix = "[id-" + std::to_string(handle_id) + "]";

#ifndef NDEBUG
  {
    std::lock_guard<std::mutex> lg(debug_log_mutex);
    std::cout << msg_prefix << " request handle start at: "
              << std::chrono::system_clock::now().time_since_epoch() /
                     std::chrono::microseconds(1)
              << "\n";
  }
#endif  // NDEBUG

  sesemi::SeMIRTWorkerRequest request =
      sesemi::DecodeSeMIRTWorkerRequest(sample_request);

  std::string prediction;
  auto ret = Execute(request, &prediction);

#ifndef NDEBUG
  {
    std::lock_guard<std::mutex> lg(debug_log_mutex);

    std::cout << msg_prefix << "prediction done at: "
              << std::chrono::system_clock::now().time_since_epoch() /
                     std::chrono::microseconds(1)
              << "\n";
  }
#endif  // NDEBUG

  std::string hex_result =
      sesemi::hex_encode(prediction.data(), prediction.size());
  if (ret == 0) {
    sesemi::SeMIRTWorkerResponse resp;
    resp.status = true;
    resp.output = hex_result;
    resp.aux = "semirt_aux";
    output->assign(resp.EncodeTo());
  } else {
    sesemi::SeMIRTWorkerResponse resp;
    resp.status = false;
    resp.output = "prediction error";
    output->assign(resp.EncodeTo());
  }

  return (ret == 0);
}

int WorkerImpl::Execute(const sesemi::SeMIRTWorkerRequest& request,
                        std::string* output) {
  if (!initialize_) return -1;
  assert(model_store_);
  int retval;
  size_t output_size;

  std::cout << "exec started at: "
            << std::chrono::system_clock::now().time_since_epoch() /
                   std::chrono::microseconds(1)
            << "\n";

  int cache_ret = -1;
  ModelContext* model_data = nullptr;
  do {
    cache_ret = model_cache.CheckAndTakeRef(request.model_name_);
  } while (cache_ret == -1);
  bool model_cache_release_needed = true;
  if (cache_ret == 0) {
    auto model_context =
        load_model(request.model_name_.c_str(), request.model_name_.size(),
                   nullptr, model_store_, &retval);
    if (!model_context) {
      model_cache.Release(request.model_name_);
      return -1;
    }
    model_data =
        model_cache.AddToCache(request.model_name_, std::move(model_context));
  } else {
    do {
      model_data = model_cache.RetrieveFromCache(request.model_name_);
    } while (!model_data);
  }

  assert(model_data);

  bool use_last_model = request.model_name_ == model_rt_id_;
  if (!use_last_model) {
    free_model_rt(model_rt_);
    model_rt_ = model_rt_init(*model_data, &retval);
    if ((!model_rt_) || (retval != 0)) {
      model_cache.Release(request.model_name_);
      ClearExecutionContext();
      return retval;
    }
  }

  model_rt_id_ = request.model_name_;

  std::unique_ptr<char[]> output_buf(new char[OUTPUT_BUF_SZ]);

  retval = execute_inference(
      request.encrypted_sample_.c_str(), request.encrypted_sample_.size(),
      *model_data, model_rt_, output_buf.get(), OUTPUT_BUF_SZ, &output_size);
  model_cache.Release(request.model_name_);
  if (retval != 0) {
    ClearExecutionContext();
    {
      std::lock_guard<std::mutex> lg(debug_log_mutex);
      std::cout << "inference error: " << retval << "\n";
    }
    return retval;
  }

  output->assign(output_buf.get(), output_size);
  {
    std::lock_guard<std::mutex> lg(debug_log_mutex);
    std::cout << "inference done at: "
              << std::chrono::system_clock::now().time_since_epoch() /
                     std::chrono::microseconds(1)
              << "\n";
  }

  return 0;
}

void WorkerImpl::Close() {
  initialize_ = false;
  if (model_store_) model_store_->Close();
  delete model_store_;
  model_store_ = nullptr;
}

}  // namespace semirt_worker
