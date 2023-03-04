# section .data
  # 这里是数据段，可以定义需要用到的变量、常量等

# section .text
  .text
  .global context_switch
  .global main

context_switch:
  # 这里是代码段，从 _start 标签开始执行
  # 可以使用各种 x86_64 汇编指令编写程序逻辑

  # 退出程序，退出码为 0
  movq $60, %rax      # sys_exit 系统调用号
  xor %rdi, %rdi     # 退出码为 deadbeef
  syscall          # 调用系统调用

main:
  jmp context_switch



