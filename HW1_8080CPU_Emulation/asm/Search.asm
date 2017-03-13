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


	; my variables to use in searching
	ORG 8000H
START:	DW 12H, 34H, 53H, 2AH, 5BH, 6FH, 33H, 21H, 7CH, 0FFH
	DW 0BAH, 0CBH, 0A1H, 1AH, 3BH, 0C3H, 4AH, 5DH, 62H, 0A3H
	DW 0B1H, 5CH, 7FH, 0CCH, 0AAH, 34H

error_msg:	DW 'error',00AH,00H ; null terminated string

	ORG 0700H
I:	DB 00H
TOTAL:	DB 26

	ORG 000DH
begin:
	LXI SP,stack 	; always initialize the stack pointer

	LXI B,START

LOOP:
	LDAX B
	SUI 3BH
	JZ FOUND
	INX B
	INX B
	LDA I
	INR A
	STA I
	SUI 26
	JNZ LOOP
NOT_FOUND:
	LXI B, error_msg
	MVI A, PRINT_STR
	call GTU_OS
	JMP EXIT

FOUND:
	hlt

EXIT:
	hlt		; end program
