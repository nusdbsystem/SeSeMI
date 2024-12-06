FROM sesemi_base_nosgx:v1 as semirt_worker_builder

USER root

# need openssl headers
RUN apt-get update && apt-get install -y libssl-dev

COPY --from=sesemi_base_nosgx:v1 /sesemi/deps /sesemi/deps
COPY ./worker/ /sesemi/worker
COPY ./message/ /sesemi/message
COPY ./ratls-channel/ /sesemi/ratls-channel
COPY ./secret/ /sesemi/secret
COPY ./server/ /sesemi/server
COPY ./store-client/ /sesemi/store-client
COPY ./utils/ /sesemi/utils

ARG INFERENCERT
ARG MODULE_OBJS_DIR=docker/worker/no-sgx 
# MODULE_OBJS_DIR is the folder that contains model_c
# give a default exist value to copy for tflm

RUN [ ${INFERENCERT} = "TFLM_DEFAULT" ] && echo "use tflm rt" \
  || (([ ${INFERENCERT} = "TVMCRT_DEFAULT" ] || [ ${INFERENCERT} = "TVMCRT_CNN" ] || [ ${INFERENCERT} = "TVMCRT_BERT" ]) && ([ -n "${MODULE_OBJS_DIR}" ] \
  && echo "use tvm c rt"))


COPY ./${MODULE_OBJS_DIR} /sesemi/model

ENV INFERENCERT ${INFERENCERT}
ENV MODULE_OBJS_DIR /sesemi/model

RUN cd /sesemi/worker \
  && make mrproper && make no_sgx && make install && cd ..

# image
FROM ubuntu:20.04

USER root

# default libuv thread pool size to 1.
ARG UV_THREADPOOL_SIZE=1
RUN echo UV_THREADPOOL_SIZE=${UV_THREADPOOL_SIZE}
ENV UV_THREADPOOL_SIZE ${UV_THREADPOOL_SIZE}

# need libcrypto.so.1.1
RUN apt-get update && apt-get install -y openssl

COPY --from=semirt_worker_builder /sesemi/worker/install /install

# default ow action so last argument 1
CMD ["/install/bin/semirt_server_no_sgx", "8080", "/mounted_store", "1"] 
