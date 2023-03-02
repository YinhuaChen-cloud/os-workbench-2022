#include "co.h"
#include <stdlib.h>

struct co {
};

// co_start(name, func, arg) 创建一个新的协程，并返回一个指向 struct co 的指针 (类似于 pthread_create)。
// * 新创建的协程从函数 func 开始执行，并传入参数 arg。新创建的协程不会立即执行，而是调用 co_start 的协程继续执行。
// * 使用协程的应用程序不需要知道 struct co 的具体定义，因此请把这个定义留在 co.c 中；框架代码中并没有限定 struct co 结构体的设计，所以你可以自由发挥。
// * co_start 返回的 struct co 指针需要分配内存。我们推荐使用 malloc() 分配。
struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  return NULL;
}

void co_wait(struct co *co) {
}

void co_yield() {
}
