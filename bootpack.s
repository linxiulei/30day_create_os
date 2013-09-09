	.file	"bootpack.c"
	.text
	.globl	DaishuMain
	.type	DaishuMain, @function
DaishuMain:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.L2:
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	DaishuMain, .-DaishuMain
	.ident	"GCC: (Gentoo 4.7.3 p1.1, pie-0.5.5) 4.7.3"
	.section	.note.GNU-stack,"",@progbits
