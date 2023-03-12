.PHONY: build-arg

LDFLAGS    += -N -Ttext-segment=0x00100000
QEMU_FLAGS += -serial mon:stdio \
              -machine accel=tcg \
              -smp "$(smp)" \
              -drive format=raw,file=$(IMAGE)

build-arg: image
	# mainargs 一般是 空
	@echo "In build-arg, mainargs = $(mainargs)"
	@( echo -n $(mainargs); ) | dd if=/dev/stdin of=$(IMAGE) bs=512 count=2 seek=1 conv=notrunc status=none

BOOT_HOME := $(AM_HOME)/am/src/x86/qemu/boot

# 同名的目标：它们共享prerequisite，所有prerequisite都满足才能编译
# 但是，下面跟着命令的目标只能有一个，参考这个网页 https://stackoverflow.com/questions/43718595/two-targets-with-the-same-name-in-a-makefile
image: $(IMAGE).elf 
	# IMAGE = amgame/build/amgame-x86_64-qemu
	@$(MAKE) -s -C $(BOOT_HOME)
	# IMAGE_REL = build/amgame-x86_64-qemu
	@echo + CREATE "->" $(IMAGE_REL)
	( cat $(BOOT_HOME)/bootblock.o; head -c 1024 /dev/zero; cat $(IMAGE).elf ) > $(IMAGE)
