# OpenWhisk deployment

We use OpenWhisk with its Docker container based invoker to run with SeSeMI components. OpenWhisk can be deployed on a kubernetes cluster with [openwhisk-deploy-kube repo](https://github.com/apache/openwhisk-deploy-kube). Here we details some important configurations.

OpenWhisk invoker needs to be configured such that it provides the appropriate flags to expose SGX drivers to the SeMIRT containers. This can be configured in `core/invoker/src/main/resources/application.conf` of the [OpenWhisk repo](https://github.com/apache/openwhisk) for `container-factory`. Then you can build the invoker image.

```conf
      extra-args: {
        "device": ["/dev/sgx_enclave:/dev/sgx/enclave"],
        "volume": ["/var/run/aesmd:/var/run/aesmd", "<path-for-the-model-storage>:/mounted_store"],
      }  
```

Another setting we set for our experiment is the timeout for idle containers

```conf
      # The "unusedTimeout" in the ContainerProxy,
      #aka 'How long should a container sit idle until we kill it?'
      idle-container = 3 minutes
```

configure the deployment helm charts by creating a yaml file to overwrite the default values. Here we provide a [sample](./ow.yaml).

Please configure the value to match the cpu cores and memory ratio of your machines and use the new invoker image that passes additional volume mount for functions to access local sgx drivers.

Label the node in kubernetes:

```sh
# for the node to host openwhisk control plane
kubectl label node <control-node> openwhisk-role=core
# for the nodes to schedule containers
kubectl label node <invoker-node> openwhisk-role=invoker
```

In the cloned `openwhisk-deploy-kube`, run `helm install owdev openwhisk/openwhisk -n openwhisk --create-namespace -f mycluster.yaml` to deploy OpenWhisk.
