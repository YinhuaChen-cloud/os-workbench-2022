NAME := amgame
# SRCS 表示为了 run 起来的 .c 代码。 AM 的相关源代码则用 AM_SRCS 表示
SRCS := $(shell find -L ./src/ -name "*.c")
export MODULE  := L0
 # := 的含义是覆盖之前的值
export AM_HOME := $(PWD)/../abstract-machine
ifeq ($(ARCH),)
# 默认架构是 x86_64-qemu
export ARCH := x86_64-qemu
endif

include $(AM_HOME)/Makefile
include ../Makefile.lab
image: git # 这个应该是用来运行 tracer-nju 的
