NAME := amgame
SRCS := $(shell find -L ./src/ -name "*.c")
export MODULE  := L0
 # := 的含义是覆盖之前的值
export AM_HOME := $(PWD)/../abstract-machine
ifeq ($(ARCH),)
export ARCH := x86_64-qemu
endif

include $(AM_HOME)/Makefile
include ../Makefile.lab
image: git
