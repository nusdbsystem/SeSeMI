FROM sesemi_base_sgx:v1 as semirt_worker_builder

USER root

COPY --from=sesemi_base_sgx:v1 /sesemi/deps /sesemi/deps
COPY ./worker/ /sesemi/worker
COPY ./message/ /sesemi/message
COPY ./ratls-channel/ /sesemi/ratls-channel
COPY ./secret/ /sesemi/secret
COPY ./server/ /sesemi/server
COPY ./store-client/ /sesemi/store-client
COPY ./utils/ /sesemi/utils

# ENV SGX_SDK=/opt/intel/sgxsdk

ARG INFERENCERT
ARG MODULE_OBJS_DIR=docker/worker/sgx 
# MODULE_OBJS_DIR is the folder that contains model_c
# give a default exist value to copy for tflm

RUN [ ${INFERENCERT} = "TFLM_DEFAULT" ] && echo "use tflm rt" \
  || (([ ${INFERENCERT} = "TVMCRT_DEFAULT" ] || [ ${INFERENCERT} = "TVMCRT_CNN" ] || [ ${INFERENCERT} = "TVMCRT_BERT" ]) && ([ -n "${MODULE_OBJS_DIR}" ] \
  && echo "use tvm c rt"))


COPY ./${MODULE_OBJS_DIR} /sesemi/model

ENV INFERENCERT ${INFERENCERT}
ENV MODULE_OBJS_DIR /sesemi/model

RUN cd /sesemi/worker \
  && make mrproper && make sgx_u sgx_t && make install && cd ..

# image
FROM sgx_dcap_ssl_2.14_1.11:v1

USER root

# default libuv thread pool size to 1.
ARG UV_THREADPOOL_SIZE=1
RUN echo UV_THREADPOOL_SIZE=${UV_THREADPOOL_SIZE}
ENV UV_THREADPOOL_SIZE ${UV_THREADPOOL_SIZE}
ENV SGX_AESM_ADDR=1

COPY --from=semirt_worker_builder /sesemi/worker/install /install

# default ow action so second last argument 1
CMD ["/install/bin/semirt_server", "8080", "/mounted_store", "1", "/install/lib/Worker_Enclave.signed.so"] 
