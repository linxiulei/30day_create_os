[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 32]
[FILE "bootpack.c"]
[SECTION .text]
	ALIGN	2
	GLOBAL	_DaishuMain
_DaishuMain:
	PUSH	EBP
	MOV	EBP,ESP
L2:
	JMP	L2
