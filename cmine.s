	.file	"cmine.c"
	.text
	.p2align 4,,15
	.globl	sig
	.type	sig, @function
sig:
.LFB67:
	.cfi_startproc
	movl	$0, running
	ret
	.cfi_endproc
.LFE67:
	.size	sig, .-sig
	.globl	__udivdi3
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align 4
.LC0:
	.string	"Totalled %llu hashes in %d seconds (%llu h/s, %llu s/coin)\n"
	.text
	.p2align 4,,15
	.globl	finalize
	.type	finalize, @function
finalize:
.LFB66:
	.cfi_startproc
	subl	$108, %esp
	.cfi_def_cfa_offset 112
	leal	72(%esp), %eax
	movl	%esi, 96(%esp)
	movl	%edi, 100(%esp)
	movl	%ebp, 104(%esp)
	movl	%ebx, 92(%esp)
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	call	gettimeofday
	movl	72(%esp), %ebp
	movl	shashes+4, %edx
	subl	time_st, %ebp
	movl	shashes, %eax
	movl	%ebp, %edi
	movl	%edx, %ecx
	sarl	$31, %edi
	movl	%ebp, %esi
	orl	%eax, %ecx
	jne	.L3
	movl	$1, %eax
	xorl	%edx, %edx
.L3:
	movl	hashes, %ecx
	movl	hashes+4, %ebx
	movl	%esi, (%esp)
	movl	%edi, 4(%esp)
	movl	%eax, 8(%esp)
	movl	%edx, 12(%esp)
	movl	%ecx, 56(%esp)
	movl	%ebx, 60(%esp)
	call	__udivdi3
	movl	56(%esp), %ecx
	movl	60(%esp), %ebx
	movl	%esi, 8(%esp)
	movl	%edi, 12(%esp)
	movl	%ecx, (%esp)
	movl	%ebx, 4(%esp)
	movl	%eax, 28(%esp)
	movl	%edx, 32(%esp)
	call	__udivdi3
	movl	60(%esp), %ebx
	movl	56(%esp), %ecx
	movl	%ebp, 16(%esp)
	movl	$.LC0, 4(%esp)
	movl	%ebx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 20(%esp)
	movl	%edx, 24(%esp)
	movl	$1, (%esp)
	call	__printf_chk
	movl	92(%esp), %ebx
	movl	96(%esp), %esi
	movl	100(%esp), %edi
	movl	104(%esp), %ebp
	addl	$108, %esp
	.cfi_def_cfa_offset 4
	.cfi_restore 5
	.cfi_restore 7
	.cfi_restore 6
	.cfi_restore 3
	ret
	.cfi_endproc
.LFE66:
	.size	finalize, .-finalize
	.section	.rodata.str1.4
	.align 4
.LC1:
	.string	"Starting miner thread %d with %s...\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"Starting miner with %s...\n"
.LC3:
	.string	"%02x"
.LC4:
	.string	"MT%d: check %s %s\n"
.LC5:
	.string	"M: check %s %s\n"
.LC6:
	.string	"a"
.LC7:
	.string	"hashes.blc"
.LC8:
	.string	"check %s %s\n"
.LC9:
	.string	"Stopping thread %d...\n"
	.text
	.p2align 4,,15
	.globl	thread
	.type	thread, @function
thread:
.LFB68:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	$-2078209981, %ebp
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$300, %esp
	.cfi_def_cfa_offset 320
	movl	%gs:20, %eax
	movl	%eax, 284(%esp)
	xorl	%eax, %eax
	movl	320(%esp), %eax
	leal	256(%esp), %esi
	movl	%esi, %ebx
	leal	281(%esp), %edi
	movl	(%eax), %eax
	movl	%eax, 44(%esp)
	.p2align 4,,7
	.p2align 3
.L7:
	call	rand
	movl	%eax, %ecx
	imull	%ebp
	movl	%ecx, %eax
	sarl	$31, %eax
	addl	%ecx, %edx
	sarl	$5, %edx
	subl	%eax, %edx
	imull	$62, %edx, %edx
	subl	%edx, %ecx
	movzbl	cl(%ecx), %eax
	movb	%al, (%ebx)
	addl	$1, %ebx
	cmpl	%edi, %ebx
	jne	.L7
	cmpl	$-1, 44(%esp)
	movb	$0, 281(%esp)
	je	.L8
	movl	44(%esp), %ecx
	movl	%esi, 12(%esp)
	movl	$.LC1, 4(%esp)
	movl	$1, (%esp)
	movl	%ecx, 8(%esp)
	call	__printf_chk
.L9:
	movl	running, %ebx
	testl	%ebx, %ebx
	je	.L10
	xorl	%eax, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	movl	%edx, %ecx
	negl	%edx
	negl	%ecx
	movl	%ecx, %ebx
	addl	%ebx, %ebx
	sall	$6, %ecx
	movl	%edx, 40(%esp)
	subl	%ecx, %ebx
	movl	40(%esp), %ecx
	movl	%ebx, 36(%esp)
	leal	192(%esp), %ebp
	leal	63(%esp), %edi
	leal	(%ecx,%edx,4), %edx
	leal	(%edx,%edx,4), %edx
	subl	%edx, %eax
	movl	%eax, 40(%esp)
	.p2align 4,,7
	.p2align 3
.L15:
	movl	36(%esp), %edx
	movl	40(%esp), %ecx
	movzbl	cl(%edx), %eax
	movb	%al, 256(%esp,%ecx)
	movl	%ebp, 8(%esp)
	movl	$25, 4(%esp)
	movl	%esi, (%esp)
	call	SHA512
	movzbl	192(%esp), %edx
	movzbl	193(%esp), %eax
	orl	%edx, %eax
	movzbl	194(%esp), %edx
	orl	%eax, %edx
	movzbl	195(%esp), %eax
	orl	%edx, %eax
	testb	$15, %al
	jne	.L11
	xorl	%ebx, %ebx
	.p2align 4,,7
	.p2align 3
.L12:
	movzbl	0(%ebp,%ebx), %eax
	movl	$.LC3, 12(%esp)
	movl	$-1, 8(%esp)
	movl	$1, 4(%esp)
	movl	%eax, 16(%esp)
	leal	(%edi,%ebx,2), %eax
	addl	$1, %ebx
	movl	%eax, (%esp)
	call	__sprintf_chk
	cmpl	$64, %ebx
	jne	.L12
	cmpl	$-1, 44(%esp)
	je	.L13
	movl	44(%esp), %ecx
	movl	%edi, 16(%esp)
	movl	%esi, 12(%esp)
	movl	$.LC4, 4(%esp)
	movl	%ecx, 8(%esp)
	movl	$1, (%esp)
	call	__printf_chk
.L14:
	movl	$.LC6, 4(%esp)
	movl	$.LC7, (%esp)
	call	fopen
	movl	%edi, 16(%esp)
	movl	%esi, 12(%esp)
	movl	$.LC8, 8(%esp)
	movl	$1, 4(%esp)
	movl	%eax, %ebx
	movl	%eax, (%esp)
	call	__fprintf_chk
	movl	%ebx, (%esp)
	call	fclose
	addl	$1, shashes
	adcl	$0, shashes+4
.L11:
	movl	running, %eax
	addl	$1, hashes
	adcl	$0, hashes+4
	testl	%eax, %eax
	jne	.L15
.L10:
	movl	44(%esp), %ecx
	movl	$.LC9, 4(%esp)
	movl	$1, (%esp)
	movl	%ecx, 8(%esp)
	call	__printf_chk
	xorl	%eax, %eax
	movl	284(%esp), %edx
	xorl	%gs:20, %edx
	jne	.L20
	addl	$300, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_restore 3
	popl	%esi
	.cfi_def_cfa_offset 12
	.cfi_restore 6
	popl	%edi
	.cfi_def_cfa_offset 8
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa_offset 4
	.cfi_restore 5
	ret
.L13:
	.cfi_restore_state
	movl	%edi, 12(%esp)
	movl	%esi, 8(%esp)
	movl	$.LC5, 4(%esp)
	movl	$1, (%esp)
	call	__printf_chk
	jmp	.L14
.L8:
	movl	%esi, 8(%esp)
	movl	$.LC2, 4(%esp)
	movl	$1, (%esp)
	call	__printf_chk
	jmp	.L9
.L20:
	call	__stack_chk_fail
	.cfi_endproc
.LFE68:
	.size	thread, .-thread
	.p2align 4,,15
	.globl	init_string
	.type	init_string, @function
init_string:
.LFB64:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	movl	$-2078209981, %edi
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %esi
	xorl	%ebx, %ebx
	.p2align 4,,7
	.p2align 3
.L22:
	call	rand
	movl	%eax, %ecx
	imull	%edi
	movl	%ecx, %eax
	sarl	$31, %eax
	addl	%ecx, %edx
	sarl	$5, %edx
	subl	%eax, %edx
	imull	$62, %edx, %edx
	subl	%edx, %ecx
	movzbl	cl(%ecx), %eax
	movb	%al, (%esi,%ebx)
	addl	$1, %ebx
	cmpl	$25, %ebx
	jne	.L22
	popl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_restore 3
	popl	%esi
	.cfi_def_cfa_offset 8
	.cfi_restore 6
	popl	%edi
	.cfi_def_cfa_offset 4
	.cfi_restore 7
	ret
	.cfi_endproc
.LFE64:
	.size	init_string, .-init_string
	.p2align 4,,15
	.globl	randomize_string
	.type	randomize_string, @function
randomize_string:
.LFB65:
	.cfi_startproc
	movzbl	cl, %edx
	movl	4(%esp), %eax
	movb	%dl, (%eax)
	ret
	.cfi_endproc
.LFE65:
	.size	randomize_string, .-randomize_string
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB69:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	andl	$-16, %esp
	subl	$48, %esp
	leal	28(%esp), %eax
	movl	$0, 4(%esp)
	leal	44(%esp), %ebx
	.cfi_offset 3, -12
	movl	%eax, (%esp)
	call	gettimeofday
	movl	28(%esp), %eax
	movl	$finalize, (%esp)
	movl	%eax, time_st
	call	atexit
	movl	$sig, 4(%esp)
	movl	$2, (%esp)
	call	signal
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	srand
	xorl	%eax, %eax
	movl	$0, 44(%esp)
	.p2align 4,,7
	.p2align 3
.L26:
	leal	36(%esp,%eax,4), %eax
	movl	%ebx, 12(%esp)
	movl	$thread, 8(%esp)
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	pthread_create
	movl	44(%esp), %eax
	addl	$1, %eax
	cmpl	$1, %eax
	movl	%eax, 44(%esp)
	jle	.L26
	movl	$0, 44(%esp)
	xorl	%eax, %eax
	.p2align 4,,7
	.p2align 3
.L27:
	movl	$0, 4(%esp)
	movl	36(%esp,%eax,4), %eax
	movl	%eax, (%esp)
	call	pthread_join
	movl	44(%esp), %eax
	addl	$1, %eax
	cmpl	$1, %eax
	movl	%eax, 44(%esp)
	jle	.L27
	xorl	%eax, %eax
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	.cfi_restore 3
	ret
	.cfi_endproc
.LFE69:
	.size	main, .-main
	.globl	shashes
	.bss
	.align 8
	.type	shashes, @object
	.size	shashes, 8
shashes:
	.zero	8
	.globl	hashes
	.align 8
	.type	hashes, @object
	.size	hashes, 8
hashes:
	.zero	8
	.globl	running
	.data
	.align 4
	.type	running, @object
	.size	running, 4
running:
	.long	1
	.comm	time_st,4,4
	.globl	cl
	.align 32
	.type	cl, @object
	.size	cl, 64
cl:
	.string	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	.zero	1
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
