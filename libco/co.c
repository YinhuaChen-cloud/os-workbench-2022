#include "co.h"
#include <stdlib.h>

// 问题1：LD_LIBRARY_PATH 是干啥用的？
// 回答：用来告诉 动态链接加载器 从哪里寻找 apps所需的动态共享库, 如果要运行的程序使用了动态链接库，需要设置这个变量才能正确运行程序

struct co {
};

// TODO: 建议在讲义中寻找更多的、关于这三个API的提示说明

// co_start(name, func, arg) 创建一个新的协程，并返回一个指向 struct co 的指针 (类似于 pthread_create)。
// * 新创建的协程从函数 func 开始执行，并传入参数 arg。新创建的协程不会立即执行，而是调用 co_start 的协程继续执行。
// * 使用协程的应用程序不需要知道 struct co 的具体定义，因此请把这个定义留在 co.c 中；框架代码中并没有限定 struct co 结构体的设计，所以你可以自由发挥。
// * co_start 返回的 struct co 指针需要分配内存。我们推荐使用 malloc() 分配。
// name: 协程名字
// func: 协程要执行的函数
// arg : 协程要执行的函数的参数
struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  return NULL;
}

// co_wait(co) 表示当前协程需要等待，直到 co 协程的执行完成才能继续执行 (类似于 pthread_join)。
// * 在被等待的协程结束后、 co_wait() 返回前，co_start 分配的 struct co 需要被释放。如果你使用 malloc()，使用 free() 释放即可。
// * 因此，每个协程只能被 co_wait 一次 (使用协程库的程序应当保证除了初始协程外，其他协程都必须被 co_wait 恰好一次，否则会造成内存泄漏)。
void co_wait(struct co *co) {
}

// co_yield() 实现协程的切换。协程运行后一直在 CPU 上执行，直到 func 函数返回或调用 co_yield 使当前运行的协程暂时放弃执行。co_yield 时
// 若系统中有多个可运行的协程时 (包括当前协程)，你应当随机选择下一个系统中可运行的协程。
void co_yield() {
}
// 关于实现寄存器切换，这里要使用内联汇编对寄存器进行直接操作。
// AbstractMachine 的实现中有一个精巧的 stack_switch_call (x86.h)，可以用于切换堆栈后并执行函数调用，且能传递一个参数，请大家完成阅读理解 (对完成实验有巨大帮助)：
// 认真学学 x86


// main 函数的执行也是一个协程，因此可以在 main 中调用 co_yield 或 co_wait。main 函数返回后，无论有多少协程，进程都将直接终止
