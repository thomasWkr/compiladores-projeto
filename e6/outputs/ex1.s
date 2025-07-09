	.file	"ex1.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	3
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	
	movl	a(%rip), %eax
	movl	%eax, -4(%rbp)
	movl	$0, %eax
	popq	%rbp
	
	ret
.LFE0:
