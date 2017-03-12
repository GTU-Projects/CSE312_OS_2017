        ; 8080 assembler code
        .hexfile Sort.hex
        .binfile Sort.com
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
stack   equ 0F000h

	org 000H
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


	ORG 4000H
    DW 012H,034H,053H,02AH,05BH,06FH, 033H, 021H, 07CH, 0FFH, 0BAH, 0CBH, 0A1H,01AH, 03BH, 0C3H, 04AH, 05DH, 062H, 0A3H, 0B1H, 05CH, 07FH, 0CCH, 0AAH, 034H

begin:
	LXI SP,stack 	; always initialize the stack pointer


	hlt		; end program
