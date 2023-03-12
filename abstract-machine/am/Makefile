# make -s -C 
# /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/../abstract-machine/am 
# archive
NAME     := am
SRCS      = $(addprefix src/, $(AM_SRCS))
INC_PATH += $(AM_HOME)/am/src

# AM_SRCS := x86/qemu/start64.S \
#            x86/qemu/trap64.S \
#            x86/qemu/trm.c \
#            x86/qemu/cte.c \
#            x86/qemu/ioe.c \
#            x86/qemu/vme.c \
#            x86/qemu/mpe.c

# NAME     := am
# MAKECMDGOALS = archive
include $(AM_HOME)/Makefile
