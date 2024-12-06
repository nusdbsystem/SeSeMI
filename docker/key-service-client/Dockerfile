FROM sesemi_base_sgx:v1 as clientbuilder

COPY --from=sesemi_base_sgx:v1 /sesemi/deps /sesemi/deps
COPY ./key-service-client/ /sesemi/key-service-client
COPY ./message/ /sesemi/message
COPY ./ratls-channel/ /sesemi/ratls-channel
COPY ./secret/ /sesemi/secret
COPY ./store-client/ /sesemi/store-client
COPY ./utils/ /sesemi/utils

RUN cd /sesemi/key-service-client && make clean && make all

# the verification still depends on some SGX libraries.
# for simplicity we used the same base image.
FROM sgx_dcap_ssl_2.14_1.11:v1

COPY --from=clientbuilder /sesemi/key-service-client/keyservice_client ./keyservice_client

CMD ./keyservice_client

# docker build -t sesemi_keyservice_client:v1 -f docker/key-service-client/Dockerfile .
