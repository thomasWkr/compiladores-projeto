    .text
    .globl	a
    .data
    .align 4
    .type	a, @object
    .size	a, 4
a:
    .long	a
    .text
    .text
    .globl	b
    .data
    .align 4
    .type	b, @object
    .size	b, 4
b:
    .long	b
    .text
    .text
    .globl	main
    .type	main, @function
main:

loadAI rbss, 0 => r0
storeAI r0 => rfp, 0
