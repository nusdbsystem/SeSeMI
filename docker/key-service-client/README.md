# Key Service Client

## build

```sh
docker build -t sesemi_keyservice_client:v1 -f docker/key-service-client/Dockerfile .
```

## run

```sh
docker run -it --name key-service-client --net host --device /dev/sgx_enclave:/dev/sgx/enclave -v <path-to-example-queries>:<in-container-path> sesemi_keyservice_client:v1 ./client <path-to-request-json> <path-to-config-json>
```
