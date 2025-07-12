    .text
    .globl	a
    .data
    .align	4
    .type	a, @object
    .size	a, 4
a:
    .zero	4
    .text
    .globl	b
    .data
    .align	4
    .type	b, @object
    .size	b, 4
b:
    .zero	4
    .text
    .globl	main
    .type	main, @function
main:
.LFB0:
    pushq	%rbp
    movq	%rsp, %rbp
    movl	$3, %eax
    movl	%eax, -4(%rbp)
    movl	$1, %eax
    movl	%eax, -4(%rbp)
    movl	$6, %eax
    movl	%eax, -8(%rbp)
    movl	-4(%rbp), %eax
    movl	$1, %ebx
    movl	%ebx, %edx
    cmpl	%eax, %edx
    setle	%dl
    movzbl	%dl, %edx
    cmp		$0, %edx
    jne .L1
    jmp .L2
.L1:
    movl	$4, %eax
    movl	%eax, -4(%rbp)
    jmp	.L3
.L2:
    movl	-4(%rbp), %eax
    movl	%eax, %eax
    jmp	.L0
.L3:
    nop
    movl	-4(%rbp), %eax
    movl	%eax, %eax
    jmp	.L0
.L0:
    popq	%rbp
    ret
.LFE0:

