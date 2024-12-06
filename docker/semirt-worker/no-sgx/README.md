# SeMIRT Worker (SGX)

Remember to change the `common/config.h` (especially output size amd buffer size) the `MODULE_OBJS_DIR` and `Enclave.config.xml`.

## build

```sh
# for tvm-default
docker build -t semirt-nosgx:v1 --build-arg INFERENCERT="TVMCRT_DEFAULT" --build-arg MODULE_OBJS_DIR=./example/tvm-mb/mobilenet -f docker/semirt-worker/no-sgx/Dockerfile .
# for tflm-default
docker build -t semirt-nosgx-tflm:v1 --build-arg INFERENCERT="TFLM_DEFAULT" -f docker/semirt-worker/no-sgx/Dockerfile .
```

## run

```sh
docker run --name worker-test -p 2053:8080 -v $PWD/worker/tmp/:/mounted_store semirt-nosgx:v1 
```

load raw worker

```sh
docker run --name worker-test -p 2053:8080 -v $PWD/worker/tmp/:/mounted_store semirt-nosgx:v1 /install/bin/semirt_server_no_sgx 8080 /mounted_store 0
```
