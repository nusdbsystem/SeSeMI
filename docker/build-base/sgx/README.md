# Base Image with dependencies pre-built

## build

```sh
docker build -t sesemi_base_sgx:v1 -f docker/build-base/sgx/Dockerfile --build-arg USE_ECDSA=1 .
```
