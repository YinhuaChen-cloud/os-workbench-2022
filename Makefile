export TOKEN   := ???

# ----- DO NOT MODIFY -----

ifeq ($(NAME),)
$(error Should make in each lab's directory)
endif

# 注意，虽然这个Makefile位于 os-workbench-2022 目录下，但它通常由子目录下的Makefile include，因此所执行的命令(比如 find)所位于的目录在子目录下
SRCS   := $(shell find . -maxdepth 1 -name "*.c" -or -name "*.S")
DEPS   := $(shell find . -maxdepth 1 -name "*.h") $(SRCS)
CFLAGS += -O1 -std=gnu11 -ggdb -Wall -Werror -Wno-unused-result -Wno-unused-value -Wno-unused-variable

.PHONY: all git test clean commit-and-make

.DEFAULT_GOAL := commit-and-make
commit-and-make: git all

$(NAME)-64: $(DEPS) # 64bit binary 
	gcc -m64 $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

$(NAME)-32: $(DEPS) # 32bit binary
	gcc -m32 $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)
 
# target 1
$(NAME)-64.so: $(DEPS) # 64bit shared library
	@echo "================ In $(NAME)-64.so target ======================="
	@echo "NAME = $(NAME)"
	@echo "DEPS = $(DEPS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "SRCS = $(SRCS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	gcc -fPIC -shared -m64 $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)
	@echo "================ Out $(NAME)-64.so target ======================="

# target 2
$(NAME)-32.so: $(DEPS) # 32bit shared library
	@echo "================ In $(NAME)-32.so target ======================="
	@echo "NAME = $(NAME)"
	@echo "DEPS = $(DEPS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "SRCS = $(SRCS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	gcc -fPIC -shared -m32 $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)
	@echo "================ Out $(NAME)-32.so target ======================="

clean:
	rm -f $(NAME)-64 $(NAME)-32 $(NAME)-64.so $(NAME)-32.so

include ../Makefile.lab
