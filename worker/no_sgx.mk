### Project Settings ###
PROJECT_ROOT_DIR ?= $(shell readlink -f .)
SESEMI_ROOT ?= $(shell readlink -f ..)
DEPS_INSTALL_DIR = $(SESEMI_ROOT)/deps/install
LIBUV_DIR = $(DEPS_INSTALL_DIR)/libuv
LLHTTP_DIR = $(DEPS_INSTALL_DIR)/llhttp

WARNING_IGNORE = -Wno-sign-compare -Wno-unused-variable -Wno-comment -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-type-limits
COMMON_FLAGS = -O2 -march=native -fpic -ftree-vectorize -Wall -Wextra $(WARNING_IGNORE)
App_Cpp_Flags = $(COMMON_FLAGS)
COMMON_INCLUDE_FLAGS = -I. -I$(PROJECT_ROOT_DIR)/include -I${SESEMI_ROOT} -I./src/no_sgx
App_Cpp_Flags += $(COMMON_INCLUDE_FLAGS)

App_Link_Flags = -lrt -pthread -lm -lcrypto -lssl
### Project Settings ###

.PHONY: all clean build_dir

all: app_no_sgx semirt_server_no_sgx

## build files needed from other directory
src/no-sgx/base64.o: $(SESEMI_ROOT)/utils/base64.cpp
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

src/no-sgx/fileutil.o: $(SESEMI_ROOT)/utils/fileutil.cpp
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

src/no-sgx/hexutil.o: $(SESEMI_ROOT)/utils/hexutil.cpp
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

src/no-sgx/json.o: $(SESEMI_ROOT)/utils/json.cpp
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

src/no-sgx/semirt_worker.o: $(SESEMI_ROOT)/message/semirt_worker.cpp
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

src/no-sgx/fs_store.o: $(SESEMI_ROOT)/store-client/fs_store.cpp
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

## build files needed from other directory

src/no-sgx/%.o: src/no-sgx/%.cpp
	@echo "GEN2 $< from $@"
	$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX <= $<"

Objects := src/no-sgx/workerimpl.o \
	src/no-sgx/base64.o \
	src/no-sgx/fileutil.o \
	src/no-sgx/hexutil.o \
	src/no-sgx/json.o \
	src/no-sgx/semirt_worker.o \
	src/no-sgx/fs_store.o

libworker.a: $(Objects)
	ar -rcs $@ $^
	@echo "LINK => $@"

app_no_sgx: src/no-sgx/app.cpp libworker.a
	$(CXX) $(App_Cpp_Flags) $< -o $@ -L. -l:libworker.a -L$(INFERENCE_RT_LINK_DIR) -l:libinference_rt.a $(App_Link_Flags)

## Build semirt server ##

Server_Additional_Include_Flags := -Iserver -I$(LIBUV_DIR)/include -I$(LLHTTP_DIR)/include -I$(SESEMI_ROOT)/server
Server_Additional_Link_Flags := -L$(LIBUV_DIR)/lib -l:libuv_a.a -L$(LLHTTP_DIR)/lib -l:libllhttp.a -lrt -ldl

server/no-sgx/service.o: $(SESEMI_ROOT)/server/service.cpp
	$(CXX) $(App_Cpp_Flags) $(Server_Additional_Include_Flags) -c $< -o $@
	@echo "CXX <= $<"

server/no-sgx/server.o: $(SESEMI_ROOT)/server/server.cpp
	$(CXX) $(App_Cpp_Flags) $(Server_Additional_Include_Flags) -c $< -o $@
	@echo "CXX <= $<"

server/no-sgx/semirt_worker.o: server/semirt_worker.cpp
	$(CXX) $(App_Cpp_Flags) $(Server_Additional_Include_Flags) -c $< -o $@
	@echo "CXX <= $<"

server/no-sgx/main.o : server/no-sgx/main.cpp
	$(CXX) $(App_Cpp_Flags) $(Server_Additional_Include_Flags) -c $< -o $@
	@echo "CXX <= $<"

Server_Objects := server/no-sgx/service.o server/no-sgx/server.o server/no-sgx/semirt_worker.o server/no-sgx/main.o

semirt_server_no_sgx: $(Server_Objects) libworker.a
	$(CXX) $(Server_Objects) -o $@ -L. -l:libworker.a $(App_Link_Flags) -L$(INFERENCE_RT_LINK_DIR) -l:libinference_rt.a $(Server_Additional_Link_Flags)

clean:
	@rm -f app_no_sgx ${Objects} libworker.a server/no-sgx/*.o semirt_server_no_sgx
