#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>

#include "worker/common/worker.h"
#include "worker/untrusted/worker_u.h"
#include "semirt_worker.h"
#include "llhttp.h"
#include "server/server.h"
#include "server/service.h"
#include "uv.h"

int main(int argc, char* argv[]) {
  // pick service selection

  if (argc != 5) {
    fprintf(stderr, "Usage: port data_path is_ow_action enclave_file\n");
    exit(1);
  }
  // parse the port
  auto port = std::stol(argv[1]);
  if (port < 0 || port > 65535) {
    fprintf(stderr, "Invalid port number\n");
    exit(1);
  }
  std::string path = argv[2];
  bool is_ow_action = std::stoi(argv[3]);
  std::string enclave_file = argv[4];
  printf("Input arguments: port: %s, model store: %s is ow action: %s\n",
         argv[1], argv[2], (is_ow_action ? "true" : "false"));

  auto model_store = sesemi::OpenFsStore(path, path + "/cache", 0);
  sesemi::Service s{
      std::unique_ptr<sesemi::ServiceWorker>(new semirt_worker::SeMIRTWorker(
          std::unique_ptr<semirt_worker::Worker>(new semirt_worker::WorkerU(
              enclave_file, std::unique_ptr<sesemi::Store>(
                                sesemi::OpenFsStore(path, path + "/cache", 0)))),
          is_ow_action))};
  sesemi::Server server(port, &s);
  if (!server.Init()) {
    fprintf(stderr, "Failed to initialize the server\n");
    exit(1);
  }
  printf("Service initialized\n");

  printf("Server starting\n");
  server.Start();

  return 0;
}

// UV_THREADPOOL_SIZE=10 SGX_AESM_ADDR=1 ./install/bin/semirt_server 2355 tmp 0 ./install/lib/Worker_Enclave.signed.so
