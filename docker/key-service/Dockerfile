FROM sesemi_base_sgx:v1 as ksbuilder

USER root

COPY --from=sesemi_base_sgx:v1 /sesemi/deps /sesemi/deps
COPY ./key-service/ /sesemi/key-service
COPY ./message/ /sesemi/message
COPY ./ratls-channel/ /sesemi/ratls-channel
COPY ./secret/ /sesemi/secret
COPY ./store-client/ /sesemi/store-client
COPY ./utils/ /sesemi/utils

RUN cd /sesemi/key-service && make mrproper && make all && make install

FROM sgx_dcap_ssl_2.14_1.11:v1

USER root

COPY --from=ksbuilder /sesemi/key-service/install /install

CMD SGX_AESM_ADDR=1 /install/bin/key_server /tmp /install/lib/KeyServer_Enclave.signed.so
