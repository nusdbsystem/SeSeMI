# SeSeMI

SeSeMI: Secure Serverless Model Inference on Sensitive Data

## Prepare SGX machines

The project has been tested with `Ubuntu 20.04` with kernel `5.13.0.41-generic`. Intel SGX: SDK/PSW 2.14 - other libraries 1.11 (matching library version is important).

The machines should be installed with `Intel SGX AESM` service and the `Intel DCAP-ECDSA` attestation infrastructure setup. refer to Intel's online [documentation](https://www.intel.com/content/www/us/en/developer/articles/technical/quote-verification-attestation-with-intel-sgx-dcap.html) to setup the SGX environment.

After the setup:

* Kernel driver `/dev/sgx_enclave` and `/sgx_provision` should be present which will be mounted to KeyService and SeMIRT containers.
* SGX aesm service launched, which will be mounted to KeyService and SeMIRT containers. Check with `systemctl status aesmd`.
* The machine should have been registered with the Intel PCCS setup in your cluster. Confirm with the DCAP sample quote generation and verfication [codes](https://github.com/intel/SGXDataCenterAttestationPrimitives/tree/DCAP_1.11/SampleCode).

## Build SeSeMI components

1. Clone this repository locally and run `make preparation` to initialize all external dependencies.
2. refer to the README under `docker` folder to build respective dockers (client, KeyService, SeMIRT).
3. the FnPacker can be launched with `main.go`.

## Sample codes

We provide a sample tvm mobilenent model and all the requests in [example](example/tvm-mb/). It includes the encrypted model, inference request and all requests to update the access info in KeyService.

Please change the KeyService address to access the KeyService you deployed in the [client configuration](example/keyservice-user/client_config.json) and [inference request](example/tvm-mb/inference_req.json).

## Setup OpenWhisk

Please follow this [guide](docker/openwhisk/README.md) to setup openwhisk cluster.

## Setup KeyService

Deploy the built key server image following the instruction in the [docker](docker/key-service/README.md) folder.

Then update the access information with [client](docker/key-service-client/README.md).

```sh
# register the user
./client register_req.json client_config.json
# specify the model decryption key
./client up_model_key_req.json client_config.json
# grant the user and worker access to the model 
./client grant_model_access_req.json client_config.json
# specify the input decryption key for the worker to use
./client add_request_key_req.json client_config.json
```

Please find the sample requests and the model in [examples](example)

## Deploy a SeMIRT for a model

Once the container is build following instructions under the [docker](docker/semirt-worker/sgx/README.md) folder of this repo. You can deploy the action directly with openwhisk client for testing.

```sh
wsk -i action create <action-name> -m <memory-budget> --docker <the SeMIRT image>
```

You can now invoke this function with the [sample request](example/tvm-mb/inference_req.json).

To create a `Fnpool` and manage its invocation through FnPacker, please refer to the FnPacker client sample codes under [fnpacker/fpcli/example.go](fnpacker/fpcli/example.go).
