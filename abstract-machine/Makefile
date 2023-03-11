# Makefile for AbstractMachine Kernels and Libraries

### *Get a more readable version of this Makefile* by `make html` (requires python-markdown)
html:
	cat Makefile | sed 's/^\([^#]\)/    \1/g' | markdown_py > Makefile.html
.PHONY: html

## 1. Basic Setup and Checks

### Default to create a  # 如果 MAKECMDGOALS 没有被定义bare-metal kernel image
# 如果 MAKECMDGOALS 没有被定义
ifeq ($(MAKECMDGOALS),)
  MAKECMDGOALS  = image
 # 那么默认目标就是 image
  .DEFAULT_GOAL = image
endif

### Override checks when `make clean/clean-all/html` -------------------------------------- start
# 下面这块东西似乎是用来检测常见错误的
# 如果 MAKECMDGOALS 不是 clean, clean-all, html， 那么这个表达式就是真
ifeq ($(findstring $(MAKECMDGOALS),clean|clean-all|html),)

# 1 MAKECMDGOALS = image
# 2 MAKECMDGOALS = archive
# 3 MAKECMDGOALS = archive
### Print build info message
# Building amgame-image [x86_64-qemu] 这三个都是下面这一行打印的
# Building am-archive [x86_64-qemu]
# Building klib-archive [x86_64-qemu]
$(info # Building $(NAME)-$(MAKECMDGOALS) [$(ARCH)])

### Check: environment variable `$AM_HOME` looks sane
ifeq ($(wildcard $(AM_HOME)/am/include/am.h),)
  $(error $$AM_HOME must be an AbstractMachine repo)
endif

### Check: environment variable `$ARCH` must be in the supported list
# 检测我们目前使用的 ARCH 是否是被支持的架构
# 2023-3-11 肉眼观看，.mk 文件有 native, x86_64-qemu, x86-qemu 三种
ARCHS = $(basename $(notdir $(shell ls $(AM_HOME)/scripts/*.mk)))
ifeq ($(filter $(ARCHS), $(ARCH)), )
  $(error Expected $$ARCH in {$(ARCHS)}, Got "$(ARCH)")
endif

### Extract instruction set architecture (`ISA`) and platform from `$ARCH`. Example: `ARCH=x86_64-qemu -> ISA=x86_64; PLATFORM=qemu`
ARCH_SPLIT = $(subst -, ,$(ARCH))
 # x86_64
ISA        = $(word 1,$(ARCH_SPLIT))
 # qemu
PLATFORM   = $(word 2,$(ARCH_SPLIT))

### Check if there is something to build
ifeq ($(flavor SRCS), undefined)
  $(error Nothing to build)
endif

### Checks end here
endif # -------------------------------------------------------------------------------------- end

## 2. General Compilation Targets

### Create the destination directory (`build/$ARCH`)
# 如果是在被别的 Makefile 文件 include 的情况下，WORK_DIR 的目录就是别的 Makefile 文件所在的目录
WORK_DIR  = $(shell pwd)
# Destination dir 就是工作目录下的 build/ARCH/
DST_DIR   = $(WORK_DIR)/build/$(ARCH)
$(shell mkdir -p $(DST_DIR))

### Compilation targets (a binary image or archive)
IMAGE_REL = build/$(NAME)-$(ARCH)
IMAGE     = $(abspath $(IMAGE_REL))
ARCHIVE   = $(WORK_DIR)/build/$(NAME)-$(ARCH).a

### Collect the files to be linked: object files (`.o`) and libraries (`.a`)
# OBJS 其实就是 DST_DIR 下的 SRCS 改后缀为 .o
OBJS      = $(addprefix $(DST_DIR)/, $(addsuffix .o, $(basename $(SRCS))))
LIBS     := $(sort $(LIBS) am klib) # lazy evaluation ("=") causes infinite recursions
LINKAGE   = $(OBJS) \
  $(addsuffix -$(ARCH).a, $(join \
    $(addsuffix /build/, $(addprefix $(AM_HOME)/, $(LIBS))), \
    $(LIBS) ))

## 3. General Compilation Flags

### (Cross) compilers, e.g., mips-linux-gnu-g++
AS        = $(CROSS_COMPILE)gcc
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
LD        = $(CROSS_COMPILE)ld
OBJDUMP   = $(CROSS_COMPILE)objdump
OBJCOPY   = $(CROSS_COMPILE)objcopy
READELF   = $(CROSS_COMPILE)readelf

### Compilation flags
INC_PATH += $(WORK_DIR)/include $(addsuffix /include/, $(addprefix $(AM_HOME)/, $(LIBS)))
INCFLAGS += $(addprefix -I, $(INC_PATH))

CFLAGS   += -O2 -MMD -Wall -Werror $(INCFLAGS) \
            -D__ISA__=\"$(ISA)\" -D__ISA_$(shell echo $(ISA) | tr a-z A-Z)__ \
            -D__ARCH__=$(ARCH) -D__ARCH_$(shell echo $(ARCH) | tr a-z A-Z | tr - _) \
            -D__PLATFORM__=$(PLATFORM) -D__PLATFORM_$(shell echo $(PLATFORM) | tr a-z A-Z | tr - _) \
            -DARCH_H=\"arch/$(ARCH).h\" \
            -fno-asynchronous-unwind-tables -fno-builtin -fno-stack-protector \
            -Wno-main -U_FORTIFY_SOURCE
CXXFLAGS +=  $(CFLAGS) -ffreestanding -fno-rtti -fno-exceptions
ASFLAGS  += -MMD $(INCFLAGS)

## 4. Arch-Specific Configurations

### Paste in arch-specific configurations (e.g., from `scripts/x86_64-qemu.mk`)
-include $(AM_HOME)/scripts/$(ARCH).mk # 从这里获取相关架构的配置

### Fall back to native gcc/binutils if there is no cross compiler
ifeq ($(wildcard $(shell which $(CC))),)
  $(info #  $(CC) not found; fall back to default gcc and binutils)
  CROSS_COMPILE :=
endif

## 5. Compilation Rules

# DST_DIR 意思就是 Destination dir, 这里 DST_DIR = /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/build/x86_64-qemu/
# %.o 匹配的是 ./src/video.o 这些, 所以使用的也是 ./src/video.c
### Rule (compile): a single `.c` -> `.o` (gcc)
# amgame/src 中的三个 .c 文件都在这里被编译
$(DST_DIR)/%.o: %.c
	@echo "In (DST_DIR)/%.o:, (DST_DIR)/%.o = $@"
	@echo "In (DST_DIR)/%.o:, $$< = $<"
	@mkdir -p $(dir $@) && echo + CC $<
	@$(CC) -std=gnu11 $(CFLAGS) -c -o $@ $(realpath $<)

### Rule (compile): a single `.cc` -> `.o` (g++)
$(DST_DIR)/%.o: %.cc
	@mkdir -p $(dir $@) && echo + CXX $<
	@$(CXX) -std=c++17 $(CXXFLAGS) -c -o $@ $(realpath $<)

### Rule (compile): a single `.cpp` -> `.o` (g++)
$(DST_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@) && echo + CXX $<
	@$(CXX) -std=c++17 $(CXXFLAGS) -c -o $@ $(realpath $<)

### Rule (compile): a single `.S` -> `.o` (gcc, which preprocesses and calls as)
$(DST_DIR)/%.o: %.S # 注意：这里的依赖只有 .S 汇编文件
	@mkdir -p $(dir $@) && echo + AS $<
	@$(AS) $(ASFLAGS) -c -o $@ $(realpath $<)

### Rule (recursive make): build a dependent library (am, klib, ...)
# am 和 klib 都使用这个规则得到
# $(LIBS) 属于 .PHONY
$(LIBS): %:
	@echo "In LIBS: %:   , $$ * = $*"
	@$(MAKE) -s -C $(AM_HOME)/$* archive
	# make -s -C /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/../abstract-machine/am archive
	# make -s -C /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/../abstract-machine/klib archive

### Rule (link): objects (`*.o`) and libraries (`*.a`) -> `IMAGE.elf`, the final ELF binary to be packed into image (ld)
$(IMAGE).elf: $(OBJS) am $(LIBS) # 猜测：这个应该是最终生成的 elf 文件的规则
	@echo + LD "->" $(IMAGE_REL).elf
	@$(LD) $(LDFLAGS) -o $(IMAGE).elf --start-group $(LINKAGE) --end-group

### Rule (archive): objects (`*.o`) -> `ARCHIVE.a` (ar)
# ARCHIVE = /home/chenyinhua/sda3/cyhos/os-workbench-2022/abstract-machine/am/build/am-x86_64-qemu.a
# ARCHIVE = /home/chenyinhua/sda3/cyhos/os-workbench-2022/abstract-machine/klib/build/klib-x86_64-qemu.a
$(ARCHIVE): $(OBJS)
	@echo + AR "->" $(shell realpath $@ --relative-to .)
	@ar rcs $(ARCHIVE) $(OBJS)

### Rule (`#include` dependencies): paste in `.d` files generated by gcc on `-MMD`
# 这个应该就是纯粹为了和 fixdep 协作提高编译速度
-include $(addprefix $(DST_DIR)/, $(addsuffix .d, $(basename $(SRCS))))

## 6. Miscellaneous

### Build order control
# image 属于 .PHONY
image: image-dep # TODO: 默认目标 ？
# make -s -C 
# /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/../abstract-machine/am 
# archive
archive: $(ARCHIVE) # <- @$(MAKE) -s -C $(AM_HOME)/$* archive 的规则
# ARCHIVE = /home/chenyinhua/sda3/cyhos/os-workbench-2022/abstract-machine/am/build/am-x86_64-qemu.a
# ARCHIVE = /home/chenyinhua/sda3/cyhos/os-workbench-2022/abstract-machine/klib/build/klib-x86_64-qemu.a
	@echo "In archive: ARCHIVE, ARCHIVE = $(ARCHIVE)"

# In image-dep, OBJS = (OBJS 可以通过 $(DST_DIR)/%.o: %.c 这个规则得到 )
# /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/build/x86_64-qemu/./src/video.o
# /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/build/x86_64-qemu/./src/keyboard.o
# /home/chenyinhua/sda3/cyhos/os-workbench-2022/amgame/build/x86_64-qemu/./src/game.o
# In image-dep, LIBS = am klib (可以通过 $(LIBS): %: 这个规则得到 )
# image-dep 属于 .PHONY
image-dep: $(OBJS) am $(LIBS)
	@echo "In image-dep, OBJS = $(OBJS)"
	@echo "In image-dep, LIBS = $(LIBS)"
	@echo \# Creating image [$(ARCH)]   # # Creating image [x86_64-qemu]
.PHONY: image image-dep archive run $(LIBS)

### Clean a single project (remove `build/`)
clean:
	rm -rf Makefile.html $(WORK_DIR)/build/
.PHONY: clean

### Clean all sub-projects within depth 2 (and ignore errors)
CLEAN_ALL = $(dir $(shell find . -mindepth 2 -name Makefile))
clean-all: $(CLEAN_ALL) clean
$(CLEAN_ALL):
	-@$(MAKE) -s -C $@ clean
.PHONY: clean-all $(CLEAN_ALL)
