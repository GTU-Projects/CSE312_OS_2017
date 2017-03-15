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
START:	DW 4,1,0,2

;START:	DW 12H, 34H, 53H, 2AH, 5BH, 6FH, 33H, 21H, 7CH, 0FFH
;	DW 0BAH, 0CBH, 0A1H, 1AH, 3BH, 0C3H, 4AH, 5DH, 62H, ;0A3H
;	DW 0B1H, 5CH, 7FH, 0CCH, 0AAH, 34H

	ORG 0700H
I:	DB 00H ; index of array(address space)
N:	DB 4 ; number of items to compare
SIZE	equ 4
MIN:	DB 03H


error_msg:	DW 'error',00AH,00H ; null terminated string




	ORG 000DH
begin:
	LXI SP,stack 	; always initialize the stack pointer

WHILE:
	LDA N ; n = size
	MVI B,0
	SUB B
	JZ EXIT ; n==0
	DCR A ; n = n-1
	STA N ;= save N


	MVI H,0 ; I=0
	LXI B,START ; load first element address to BC
FOR:
	LDA N ;= load N
	SUB H ; A = N-H
	JZ WHILE ; A-I == 0


IF:
	LDAX B ; get a[i]
	MOV D,A ; D = a[i]
	INX B
	INX B
	LDAX B ; get a[i+1]

	SUB D ; a[i+1] - a[i]
	JM SWAP ; if A=a[i+1] < D=a[i]
	JMP CONT_FOR ; go on

SWAP:
	LDAX B ; a = arr[i+1]
	MOV E,A ; e = a
	MOV A,D ; a= d, d=arr[i]
	STAX B ; arr[i+1] = d
	DCX B
	DCX B
	MOV A,E ; a = e
	STAX B ; arr[i] = e

CONT_FOR:
	INR H
	INX B
	INX B
	JMP FOR



EXIT:
	hlt		; end program
