### Project Settings ###
PROJECT_ROOT_DIR ?= $(shell readlink -f ../..)
SESEMI_ROOT ?= $(shell readlink -f ../../..)
DEP_LOCAL_DIR := deps

App_C_Flags := -Wall -Wextra -O2 -march=native -fPIC
App_Cpp_Flags := -Wall -Wextra -O2 -march=native -fPIC
COMMON_INCLUDE_FLAGS :=	-I./src -I$(CRT_SRCS_LOCAL)/include
App_C_Flags += $(COMMON_INCLUDE_FLAGS)
App_Cpp_Flags += $(COMMON_INCLUDE_FLAGS)
### Project Settings ###

.PHONY: all install clean mrproper

all: lib/libtflm.a

src/no_sgx/tflm_patch.o: src/no_sgx/patch.cpp
	$(CXX) $(App_Cpp_Flags) -Isrc/common -c $< -o $@
	@echo "CXX   <=  $<"

Objects := src/no_sgx/tflm_patch.o

lib/libtflm.a: $(Objects)
	cp $(DEP_LOCAL_DIR)/lib/libtflm.a $@
	ar r $@ $^
	@echo "LINK => $@"

clean:
	@rm -f src/no_sgx/*.o
	@rm -f lib/libtflm.a
