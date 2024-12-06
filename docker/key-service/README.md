# Key Service

## build

```sh
docker build -t sesemi_keyservice:v1 -f docker/key-service/Dockerfile .
```

## run

```sh
docker run -it --name key-server --net host --device /dev/sgx_enclave:/dev/sgx/enclave -v /var/run/aesmd:/var/run/aesmd sesemi_keyservice:v1
```
