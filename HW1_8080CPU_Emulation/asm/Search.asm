        ; 8080 assembler code
        .hexfile Search.hex
        .binfile Search.com
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

error_msg:	dw 'error',00AH,00H ; null terminated string

begin:
	LXI SP,stack 	; always initialize the stack pointer

	jmp NOT_FOUND

NOT_FOUND:
	LXI B, error_msg
	MVI A, PRINT_STR
	call GTU_OS
	JMP EXIT

EXIT:
	hlt		; end program
