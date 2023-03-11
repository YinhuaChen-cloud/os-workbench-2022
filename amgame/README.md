# L0: 直接运行在硬件上的小游戏

# 注意！！！！！！！

直接在 amgame 目录下运行 make run，可以看到已经能够运行键盘和VGA了！！！！

## 1. 背景 -- checked

## 2. 实验描述 -- checked

### 2.1. 阅读 AbstractMachine 文档 -- checked

想知道为什么能用你熟悉的方式编写操作系统？阅读 AbstractMachine 的文档。当然，文档很多，你不必一次看完，建立了一些基本概念后，就可以试着从样例代码出发开始编程了。另一个好消息是我们的实验框架中直接包含了 AbstractMachine 的代码并在 Makefile 中完成了配置 (RTFSC)，从而你无需额外配置/下载。 (TODO: Makefile 中完成了 AbstractMachine 的代码配置)

### 2.1.实现 AbstractMachine 中 klib 中缺失的函数 -- checked

```
⚠️ 本项目为选做 
如果你对编程感到很苦恼，不妨只实现一些简化版本的库，无需全部实现；随着实验的进展后续补齐。
```

虽然我们已经为大家搭建好了 C 语言代码在 bare-metal 上执行的环境，但你一定不想只用 putch 打印吧！这会使你连打印一个整数都麻烦到怀疑人生。<font color="red">实际上，AbstractMachine 框架代码中包含一个基础运行库的框架 klib，其中包含了方便你编写 “bare-metal” 程序的系列库函数，例如 assert, printf, memcpy 等。</font>我们不强制你实现所有 klib 函数，但随着操作系统实验的进展，你会体会到不实现库函数的苦头。
(TODO: 看看 klib 在哪里？好像可以直接复制PA中实现的代码哦！)

如果你在《计算机系统基础》中已经完成了实现，你可以直接复制你的实现 (但要小心！你的库实现可能有 bug，或是不能在多处理器的情况下工作)。这部分代码非常重要——会一直使用到这学期的最后，因此也请你非常小心地编写代码，例如编写相当多的 assertions；klib 中的 bug 会使原本就已很困难的操作系统内核调试更加令人沮丧。

```
❓ Assertions?
库函数里还能给什么 assertions？这里的学问可大了。实际上，C 语言里的每一个变量都有它的 “含义”，例如都是指针，甚至是 void * 类型的指针，有些可能是指向对象的，有些则是指向代码的。在 “不该发生” (例如一个本应该为正数的值给成了负数，或是 memcpy 复制了超过 16 MB 的数据等) 的事情上给出警告或 assert 错误，能极大地帮助你尽早发现问题。

当然，这件事早已被学术界注意到，例如 refinement type 和 Daikon invariant detector。虽然你未必会用这些研究工具，但其中的思想会极大地影响你的编程方式。
```

### 2.2. 实现可移植的、直接运行在计算机硬件上的小游戏 (必做) (TODO: 这里的内容建议边做边看) -- checked

⚠️ 这是一个送分实验
只要程序不崩溃，哪怕只有几个像素点在屏幕上乱动也可以 (就可以通过 Online Judge)。但大家也可以根据自己的兴趣实现一些简单的游戏。试图内卷 (实现精美的游戏) 不会使你得到更多的分数。

你需要编写一个直接运行在 AbstractMachine 上 (<font color="red">仅仅启用 IOE 扩展</font>，不使用其他硬件机制如中断/异常/虚拟存储/多处理器) 的小游戏；满足：

1. 有肉眼可辨认的图形输出；
2. 能使用键盘与游戏交互；
3. 游戏使用的内存 (代码、静态数据总和不超过 1 MiB、堆区 heap 使用不超过 1 MiB)，因此你不必考虑复杂的图形；
4. 按 ESC 键后调用 halt() 退出；除此之外游戏程序永不调用 halt() 结束，Game Over 后按键重新开始。

虽然不限实现什么游戏 (简单到弹球、贪吃蛇等 100 行以内搞定的游戏)，但你依然需要在可移植性/通用性上对你的代码做出一定的保证：

* 兼容很简单的处理器：即小游戏运行中只调用 TRM 和 IOE API，而不使用其他 API，且——这样大家的游戏就可以在各个 CPU 上广为流传下去啦！
* 你的游戏应当是可以在多个硬件体系结构之间移植的，考虑兼容以下情况：
  * 适配不同的屏幕大小。不同的体系结构中，屏幕大小可能是 320×200、640×480、800×600 等，你的游戏最好能在所有分辨率下都获得不错的体验；
  * 同 minilabs 一样，你的程序可能运行在 32/64-bit 平台，因此你应当使用 intptr_t 或 uintptr_t 来保存指针数值；
  * 兼容大/小端，因此禁止把不同大小的指针类型强制转换；

我们会在 x86_64-qemu (64-bit) 和 x86-qemu (32-bit) 两个环境下运行你的游戏，你的游戏必须在两个环境下都能正确编译并运行良好。此外，AbstractMachine 假设 bare-metal 不支持浮点数指令。在 x86_64 的编译选项中，我们添加了 -mno-sse。尽管有浮点数的程序在 ARCH=native 下可以正确编译运行，但可能在其他架构下失效。这么做的目的是使 AbstractMachine 程序能运行在各种 (简化) 的处理器上；尤其是同学们自己编写的处理器——也许下个学期你就可以在自己的 CPU 上运行你自己的游戏了！

在我们的 am-kernels 项目中包含了一些运行在 AbstractMachine 上的代码，例如 NES 模拟器 LiteNES，欢迎大家参考——LiteNES 只使用了 IOE 的 API，和这个实验中你们所需要实现的类似。

```
可选项：考虑游戏性能
你的游戏可能运行在频率仅有 10MHz 的自制处理器上。因此，如果你希望你的游戏能在未来你自己写的处理器上流畅运行，减少每一帧的运算量和图形绘制就显得比较重要了。你可以计算出一个时钟周期大致允许运行的代码量 (假设绘制一个像素也需要若干条指令)，相应设计游戏的逻辑。

我们认为优秀的游戏会被收录到游戏池中，永久保存下来！
```

## 3. 正确性标准 (TODO: 边做边看) -- checked

本实验对游戏不做任何要求：你只需要响应键盘输入、输出变化的图形即可。但 Online Judge 会严格按照上面的要求来评测：

1. 通过 qemu 的 -m 选项限制内存的大小；使用超过规定的内存可能导致程序 crash；
2. 在不同的环境下 (x86-qemu 或 x86_64-qemu, native) 运行你的程序，并且可能修改屏幕的分辨率；
3. 随机发送一定的键盘事件。
但 Online Judge 不试图理解游戏的逻辑，只做基本的正确性检查：

1. 除非按 ESC 键，我们假设游戏程序不结束；如果检测到 halt() 的调用 (包括 assert 失败，我们可能会增加一些额外的合法性检查)，将判定为错 (Runtime Error 或 Wrong Answer)；
2. 如果按 ESC 键后游戏不 halt()，将判定为错；
3. 如果虚拟机或进程发生崩溃、非预期的重启等 (由 undefined behavior 导致，例如访问某非法内存可能导致异常或 CPU Reset)，则被判定为错；
4. 其他违反 specifications 的问题，如在 ioe_init() 之前调用 io_read/io_write，将被判定为错。

此外，我们会链接我们 klib 的参考实现，因此你不必担心你的 klib 有些许问题；但你要保持 klib 库函数的行为与 libc 一致。注意 native 会链接系统本地的 glibc (而非 klib)。

## 4. 实验指南

### 4.1. 实现库函数

实现库函数是普通的 C 语言练习题；互联网上也有很多代码可以参考，但不要照抄哦！值得注意的是，编写可移植代码时，尽量使用 C 标准提供的机制，而不是做一些对硬件的假设，例如在 i386 平台上的 printf 有以下 “高明” (其实并不高明) 的实现方法 (请你花时间理解一下)：

```c
int printf(const char *fmt, ...) {
  int *args = (int *)&fmt + 1;
  //          (int *)(&fmt + 1) also works; why?
  // args[0], args[1], ... 分别是第 1, 2, ... 个可变参数
}
```

上述代码其实做了一些 i386 特定的假设：

所有参数构成一个 (堆栈上的) “数组”，因此可以用 &fmt + 1 得到剩余参数的起始地址；
int 类型的长度与指针的长度相同。
但在 x86-64 上，上述两个假设都不再成立，前 6 个参数用寄存器传递，且 sizeof(int) 通常是 4。可移植的方式是使用 stdarg.h 中的 va_list, va_start, va_arg。机器相关的假设、数据类型大小的假设 ((int *)ptr 在 64-bit 平台上将会报错) 等都可能引起移植性问题。

注意你可以在本地通过修改 Makefile 绕过某些 warnings，使你的程序看起来正确地在多个平台上运行；但 Online Judge 会使用我们的 Makefile 和 AbstractMachine 实现，并且可能经过一定的修改 (例如设置为不同的屏幕分辨率)。

### 4.2. 访问 I/O 设备
没有库函数的 C 语言程序类似于状态机，只能完成纯粹的 “计算”。TRM 中唯一能够和外界交互的手段是 putch() 在调试终端上打印字符和 halt() 终止程序。我们的硬件提供了若干 I/O 设备，AbstractMachine 可以通过 IOE 访问它们。在调用 I/O 设备之前，需要调用 ioe_init() 初始化，然后就可以用

```c
void ioe_read (int reg, void *buf);
void ioe_write(int reg, void *buf);
```

两个函数访问 AbstractMachine 中的 I/O “寄存器” 了。详情请参考 AbstractMachine 文档和 amgame 框架代码。在 klib-macros.h 中包含了更简化的 I/O 寄存器访问方法 io_read 和 io_write，请大家查看。用这组 API 你就可以省去手工定义变量的麻烦，例如直接

```c
int width = io_read(AM_GPU_CONFIG).width;
```

到这里，大家可能会产生的一个疑问是：运行在 “裸机” 上的程序可以用哪些标准库？我们知道，libc 中相当一部分函数都调用操作系统，例如 printf, malloc 等，即便引用了 stdio.h 这样的头文件，它们的实现依然是缺失的；另一方面，我们引用了一些库的头文件，例如 stdint.h (包含诸如 int32_t 这些类型的定义)、stdarg.h 等，却可以正常工作。这是为什么？

事实上，AbstractMachine 的程序运行在 freestanding 的环境下 (操作系统上的 C 程序运行在 hosted 环境下)：The __STDC_HOSTED__ macro expands to 1 on hosted implementations, or 0 on freestanding ones. The freestanding headers are: float.h, iso646.h, limits.h, stdalign.h, stdarg.h, stdbool.h, stddef.h, stdint.h, and stdnoreturn.h. You should be familiar with these headers as they contain useful declarations you shouldn't do yourself. GCC also comes with additional freestanding headers for CPUID, SSE and such.

这些头文件中包含了 freestanding 程序也可使用的声明。有兴趣的同学可以发现，可变参数经过预编译后生成了类似 __builtin_va_arg 的 builtin 调用，由编译器翻译成了特定的汇编代码。

### 4.3. 实现游戏
在这里，我们提供一个与 amgame 不同的框架代码，主循环使用轮询 (polling) 中不断等待下一帧时刻的到来，一旦到来，就读取键盘按键并处理，然后模拟这一帧中游戏发生的事情。

```c
next_frame = 0;
while (1) {
  while (uptime() < next_frame) ; // 等待一帧的到来
  while ((key = readkey()) != AM_KEY_NONE) {
    kbd_event(key);         // 处理键盘事件
  }
  game_progress();          // 处理一帧游戏逻辑，更新物体的位置等
  screen_update();          // 重新绘制屏幕
  next_frame += 1000 / FPS; // 计算下一帧的时间
}
```

大家见过的 LiteNES 和仙剑奇侠传，都是这样的循环。我们以弹球游戏为例，你只需要维护弹球的坐标  .... (看原网页)

即可，加上以下两点就是很酷的弹球游戏啦！

1. 使用键盘 (如上下左右) 可以修改  ... (原网页)
2. 弹球到屏幕边缘时，相应方向的速度符号取反，即 “完全弹性反射”。

```
⚠️ 再次提示
这个实验的目的是让大家熟悉 AbstractMachine 编程环境 (将会使用一整个学期)。只要你的游戏满足最低标准 (附有正常的实验报告)，就可以得到满分。但请你务必独立完成实验，例如找到 Online Judge 报告的错误。
```

### 4.4. 小游戏和操作系统 (OS和游戏的运行原理有点相似)
最后，为什么我们要在第一个实验里做一个游戏？首先，这是为了有一个给分的理由，能让大家都通过。其实还是因为游戏和操作系统的工作原理有那么一点相似。大家不妨可以看一下课程发布代码中的 NES 模拟器 LiteNES，简化的模拟器 (游戏) 的 “主循环” (fce.c) 代码如下：

```c
while (1) {
  // 在每一个时间片，例如每 16.7ms (60 fps)
  wait_for_frame();

  // 做完这一个时间片内需要完成的工作
  int scanlines = 262;
  while (scanlines-- > 0) {
    ppu_cycle();      // 更新游戏图像
    psg_detect_key(); // 读取按键，更新游戏逻辑
  }
}
```

受到上面代码的启发，我们不妨在程序的主循环里不再是一次执行一帧，而在 “每一帧” 中都执行另一个程序，程序执行完后返回主循环。因此类似于游戏，批处理系统也是一个大循环：

```c
while (1) {
  // 等待键盘输入需要运行的命令
  Job *job = wait_for_job();

  // 加载并执行命令
  load(job);
}
```

不妨假设 “批处理” 系统是这样工作的：

1. 批处理系统执行的命令由键盘输入，因此 wait_for_job() 就是从键盘读取按键并解析成命令，和游戏读取按键类似。
2. 执行的命令 (job) 是保存在磁盘上的 ELF 格式的二进制文件，使用硬件提供的 I/O 指令，从存储设备中将二进制文件加载到内存中。(实验代码中有一份 x86/x86_64 通用的二进制文件加载代码，位于 abstract-machine/am/src/x86/qemu/boot/main.c）；简化的代码如下：

```c
copy_from_disk(elf32, 4096, 0); // 从磁盘读取 ELF 文件头
if (elf->e_machine == EM_386) {  // x86 (32-bit)
  load_elf32(elf32); // 从磁盘加载二进制文件到 ELF 文件描述的位置
  ((void(*)())(uint32_t)elf32->e_entry)(); // 跳转到加载的代码执行
}
```

跳转后，控制权就交给了 job；job 结束后函数返回，重新回到批处理系统——嘿！我们已经在第一个实验里体验了一个 “简易” 的操作系统！原来实现操作系统也没那么可怕嘛。从下个实验开始将正式开启多处理器，开始现代操作系统的旅程。





