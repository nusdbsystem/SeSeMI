# SeMIRT Worker (SGX)

Remember to change the `common/config.h` (especially output size and buffer size) the `MODULE_OBJS_DIR` and `Enclave.config.xml`.

For TVM. We use its model convertion capabilities from Apache MXNET. Here we provide an example mobilenet model from mxnet compiled into TVM c binaries: [here](../../../example/tvm-mb/mobilenet/)

## build

```sh
# for tvm-default
docker build -t semirt:v1 --build-arg INFERENCERT="TVMCRT_DEFAULT" --build-arg MODULE_OBJS_DIR=./example/tvm-mb/mobilenet -f docker/semirt-worker/sgx/Dockerfile .
# for tflm-default
docker build -t semirt-tflm:v1 --build-arg INFERENCERT="TFLM_DEFAULT" -f docker/semirt-worker/sgx/Dockerfile .
```

## run

```sh
docker run --name worker-test -p 2053:8080 -v $PWD/worker/tmp/:/mounted_store --device /dev/sgx_enclave:/dev/sgx/enclave -v /var/run/aesmd:/var/run/aesmd semirt:v1 
```

load raw worker

```sh
docker run --name worker-test -p 2053:8080 -v $PWD/worker/tmp/:/mounted_store --device /dev/sgx_enclave:/dev/sgx/enclave -v /var/run/aesmd:/var/run/aesmd semirt:v1 /install/bin/semirt_server 8080 /mounted_store 0 /install/lib/Worker_Enclave.signed.so
```

send a request

```sh
curl -X POST localhost:2053/run -H 'Content-Type: application/json' -d @example/tvm-mb/ow_run_req.json
```
