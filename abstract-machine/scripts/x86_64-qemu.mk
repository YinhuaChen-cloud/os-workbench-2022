include $(AM_HOME)/scripts/isa/x86_64.mk
# export CROSS_COMPILE := x86_64-linux-gnu-
# CFLAGS  += -m64 -fPIC -mno-sse
# ASFLAGS += -m64 -fPIC
# LDFLAGS += -melf_x86_64

include $(AM_HOME)/scripts/platform/qemu.mk
# .PHONY: build-arg

# LDFLAGS    += -N -Ttext-segment=0x00100000
# QEMU_FLAGS += -serial mon:stdio \
#               -machine accel=tcg \
#               -smp "$(smp)" \
#               -drive format=raw,file=$(IMAGE)

# build-arg: image
# 	@( echo -n $(mainargs); ) | dd if=/dev/stdin of=$(IMAGE) bs=512 count=2 seek=1 conv=notrunc status=none

# BOOT_HOME := $(AM_HOME)/am/src/x86/qemu/boot

# image: $(IMAGE).elf
# 	@$(MAKE) -s -C $(BOOT_HOME)
# 	@echo + CREATE "->" $(IMAGE_REL)
# 	@( cat $(BOOT_HOME)/bootblock.o; head -c 1024 /dev/zero; cat $(IMAGE).elf ) > $(IMAGE)

AM_SRCS := x86/qemu/start64.S \
           x86/qemu/trap64.S \
           x86/qemu/trm.c \
           x86/qemu/cte.c \
           x86/qemu/ioe.c \
           x86/qemu/vme.c \
           x86/qemu/mpe.c

# TODO: 我猜测这就是用来运行的代码
run: build-arg
	@qemu-system-x86_64 $(QEMU_FLAGS)
