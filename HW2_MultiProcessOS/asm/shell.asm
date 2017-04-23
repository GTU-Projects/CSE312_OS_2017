        ; 8080 assembler code
        .hexfile shell.hex
        .binfile shell.com
        ; try "hex" for downloading in hex format
        .download bin
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
FORK		equ 7
EXEC		equ 8
WAITPID		equ 9

	; Position for stack pointer
stack   equ 0F000H

	org 0000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE

	; my variables to use in searching
	ORG 8000H

in_take_msg:	DW '##-->Enter file name',00AH,00H ; null terminated string
bye_msg:	DW '##-->good bye',00AH,00H
out_hello:	DW '##-->Welcome',00AH,00H

begin:
	LXI SP,stack 	; always initialize the stack pointer

	LXI B,out_hello
	MVI A, PRINT_STR ; store system call type
	call GTU_OS ; system call

	MVI B, 70
	MVI C, 70
	MVI A, READ_STR
	call GTU_OS	

	MVI A, FORK ; store system call type
	call GTU_OS ; system call


	; READ USE AGE AND PRINT SCREEN
	LXI B,bye_msg ; test for PRINT_STR
	MVI A, PRINT_STR ; store system call type
	call GTU_OS ; system call


	hlt    ; end program