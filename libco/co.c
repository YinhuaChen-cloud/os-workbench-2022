#include "co.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define panic(...) printf(__VA_ARGS__); assert(0);

typedef enum Co_STATE { // 协程状态 RUNNABLE ...
  RUNNABLE,
  RUNNING,
  WAITING,
  DONE
} Co_STATE;

struct co {
  // 寄存器状态

  // 协程状态
  Co_STATE state;
  // 链表上一个
  struct co* prev;
  // 链表下一个
  struct co* next;
};

// 用来存放所有协程Co_STATE结构体的链表
struct co*link_head = NULL;
struct co*link_tail = NULL;
int link_size = 0; // 链表大小
#define LINK_MAXSIZE 128 // 链表最大长度为128

// co_start(name, func, arg) 创建一个新的协程，并返回一个指向 struct co 的指针 (类似于 pthread_create)。
// * 新创建的协程从函数 func 开始执行，并传入参数 arg。新创建的协程不会立即执行，而是调用 co_start 的协程继续执行。
// * 使用协程的应用程序不需要知道 struct co 的具体定义，因此请把这个定义留在 co.c 中；框架代码中并没有限定 struct co 结构体的设计，所以你可以自由发挥。
// * co_start 返回的 struct co 指针需要分配内存。我们推荐使用 malloc() 分配。
// name: 协程名字
// func: 协程要执行的函数
// arg : 协程要执行的函数的参数
// co_start 会在共享内存中创建一个新的状态机 (堆栈和寄存器也保存在共享内存中)，仅此而已。

// 新状态机的 %rsp 寄存器应该指向它独立的堆栈，%rip 寄存器应该指向 co_start 传递的 func 参数。
// 根据 32/64-bit，参数也应该被保存在正确的位置 (x86-64 参数在 %rdi 寄存器，而 x86 参数在堆栈中)。
// main 天然是个状态机，就对应了一个协程；

// struct co *thd1 = co_start("thread-1", work, "X"); 
struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  // 1. 创建一个状态机，状态被存在 struct co 结构体中
  struct co* p = malloc(sizeof(struct co));
  // 对状态机状态进行初始化
  p->state = RUNNABLE;
  p->prev = NULL; 
  p->next = NULL; 

  // 2. 把这个新的结构体存放于链表里, 供后续调度
  if(link_tail) { // 如果此时链表不为空，把新增节点放到链表尾部
    link_tail->next = p;
    p->prev = link_tail;
    link_tail = p;  // 更新链表尾部
    link_size++;
    assert(link_size <= LINK_MAXSIZE);
  } else { // 如果此时链表为空, 则链表头和链表尾都是这个新增节点 TODO: 我们好像没有进行删除链表的操作
    link_head = p;
    link_tail = p;
    link_size++;
  }

  return p; // 程序执行流回到 test1 里
}

// co_wait(co) 表示当前协程需要等待，直到 co 协程的执行完成才能继续执行 (类似于 pthread_join)。
// * 在被等待的协程结束后、 co_wait() 返回前，co_start 分配的 struct co 需要被释放。如果你使用 malloc()，使用 free() 释放即可。
// * 因此，每个协程只能被 co_wait 一次 (使用协程库的程序应当保证除了初始协程外，其他协程都必须被 co_wait 恰好一次，否则会造成内存泄漏)。

// co_wait 会等待状态机进入结束状态，即 func() 的返回。感觉这里要进行一个进程切换？
void co_wait(struct co *co) {
  // 只有在 co 所指向的状态机被移除了，这个函数才会返回
  while(co->state != DONE) {
    // 如果 co 的状态不为 DONE, 则继续进行调度（切换协程（main本身也是个协程））
    co_yield();
  }
  // 释放已经 DONE 了的协程，并且把它从链表中取出
  if(co->prev && co->next) {
    co->prev = co->next;
    co->next = co->prev
  }
  else if(!(co->prev) && co->next) { // 链表头
    link_head = co->next;
    link_head->prev = NULL;
  }
  else if(co->prev && !(co->next)) { // 链表尾
    link_tail = co->prev;
    link_tail->next = NULL;
  }
  else { // 只剩下这一个节点了
    link_head = NULL;
    link_tail = NULL;
  }
  free(co);
}

// co_yield() 实现协程的切换。协程运行后一直在 CPU 上执行，直到 func 函数返回或调用 co_yield 使当前运行的协程暂时放弃执行。co_yield 时
// 若系统中有多个可运行的协程时 (包括当前协程)，你应当随机选择下一个系统中可运行的协程。

// co_yield 会将当前运行协程的寄存器保存到共享内存中，然后选择一个另一个协程，将寄存器加载到 CPU 上，就完成了 “状态机的切换”；
void co_yield() {
  panic("co_yield not implemented yet\n");
}
// 关于实现寄存器切换，这里要使用内联汇编对寄存器进行直接操作。
// AbstractMachine 的实现中有一个精巧的 stack_switch_call (x86.h)，可以用于切换堆栈后并执行函数调用，且能传递一个参数，请大家完成阅读理解 (对完成实验有巨大帮助)：
// 认真学学 x86


// main 函数的执行也是一个协程，因此可以在 main 中调用 co_yield 或 co_wait。main 函数返回后，无论有多少协程，进程都将直接终止

