#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  jmp_buf env; // 这是一个数组类型，用于存储当前的执行状态
  int i;

  i = setjmp(env); // 保存当前的程序执行状态，包括栈、寄存器等，并返回一个标记，这个标记可以用于后续的跳转
  printf("i = %d\n", i);
  if(i != 0 && i != 2) exit(0); // setjmp 返回0表示直接调用它的函数返回, 非0值表示通过longjmp函数跳转到了setjmp的调用点

  longjmp(env, 2); // 用来跳转到 env 变量所保存的位置。第二个参数似乎是用来设置跳到的 setjmp 函数的返回值，这个返回值不能为0, 程序员设置为0时，它会返回1
  printf("This line does not get printed\n");

}


