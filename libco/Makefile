# make -s which I guess it's "all"
NAME := $(shell basename $(PWD))# libco
export MODULE := M2 # := 是覆盖之前的值
all: $(NAME)-64.so $(NAME)-32.so # libco-64.so libco-32.so 
CFLAGS += -U_FORTIFY_SOURCES # 减少编译时间和运行时检测时间

include ../Makefile
