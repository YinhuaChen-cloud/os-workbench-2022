#  .data
  # 这里是数据段，可以定义需要用到的变量、常量等

  .text
  .global context_switch

context_switch:
  # 把所有通用寄存器存放到 rdi 指向的 context 中
  # 16个64位寄存器：
  # %rax, %rbx, %rcx, %rdx, %rsi, %rdi, %rsp, %rbp, %r8, %r9, %r10, %r11, %r12, %r13, %r14, %r15
  movq %rax, 0(%rdi)
  movq %rbx, 8(%rdi)
  movq %rcx, 16(%rdi)
  movq %rdx, 24(%rdi)

  movq %rsi, 32(%rdi)
  movq %rdi, 40(%rdi)
  movq %rsp, 48(%rdi)
  movq %rbp, 56(%rdi)

  movq %r8,  64(%rdi)
  movq %r9,  72(%rdi)
  movq %r10, 80(%rdi)
  movq %r11, 88(%rdi)

  movq %r12, 96(%rdi)
  movq %r13, 104(%rdi)
  movq %r14, 112(%rdi)
  movq %r15, 120(%rdi)

  # 把所有通用寄存器恢复成 rsi 指向的 context
  movq 0(%rsi)  , %rax
  movq 8(%rsi)  , %rbx
  movq 16(%rsi) , %rcx
  movq 24(%rsi) , %rdx
                  
  movq 40(%rsi) , %rdi
  movq 48(%rsi) , %rsp
  movq 56(%rsi) , %rbp
                  
  movq 64(%rsi) , %r8
  movq 72(%rsi) , %r9
  movq 80(%rsi) , %r10
  movq 88(%rsi) , %r11
                  
  movq 96(%rsi) , %r12
  movq 104(%rsi), %r13
  movq 112(%rsi), %r14
  movq 120(%rsi), %r15
  # rsi 自己最后做修改
  movq 32(%rsi) , %rsi

  retq # 返回(实际上是切换到另一个协程去了)
