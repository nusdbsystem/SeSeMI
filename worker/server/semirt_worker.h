#ifndef SESEMISERVICE_H_
#define SESEMISERVICE_H_

#include <mutex>

#include "worker/common/worker.h"
#include "server/worker.h"

namespace semirt_worker {

class SeMIRTWorker : public sesemi::ServiceWorker {
 public:
  SeMIRTWorker(std::unique_ptr<Worker>&& worker, bool is_ow_action = false)
      : initialized_(false),
        is_ow_action_(is_ow_action),
        worker_(std::move(worker)){};
  ~SeMIRTWorker();

  // delete copy and move
  SeMIRTWorker(const SeMIRTWorker&) = delete;
  SeMIRTWorker(SeMIRTWorker&&) = delete;
  SeMIRTWorker& operator=(const SeMIRTWorker&) = delete;
  SeMIRTWorker& operator=(SeMIRTWorker&&) = delete;

  bool Initialize() override;
  bool Handle(const std::string& url, const std::string& input,
              std::string* output) override;
  void Close() override;

 private:
  bool initialized_;
  bool is_ow_action_;
  std::unique_ptr<Worker> worker_;
};

}  // namespace semirt_worker

#endif  // SESEMISERVICE_H_
