        ; 8080 assembler code
        .hexfile sortv3.hex
        .binfile sortv3.com
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
	; this file sorts numbers with bubble sort

	; my variables to use in searching
	ORG 1000H ; 1K
START:	DW 12H, 34H, 53H, 2AH, 5BH, 6FH, 33H, 21H, 7CH, 0FFH,0BAH, 0CBH, 0A1H, 1AH, 3BH, 0C3H, 4AH, 5DH, 62H,0A3H,0B1H, 5CH, 7FH, 0CCH, 0AAH, 34H


	ORG 0700H
N:	DB 26 ; number of items to compare
I:	DB 00H ; index for print array

	ORG 000DH
begin:
	LXI SP,stack 	; always initialize the stack pointer

WHILE:
	LDA N ; n = size
	DCR A ; n = n-1

	JZ EXIT ; n==0

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
	INX B ; increment array index
	INX B
	LDAX B ; get a[i+1]

	CMP D ; a[i+1] - a[i]
	JC SWAP ; if A=a[i+1] < D=a[i]
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
	INX B
	INX B


CONT_FOR:
	INR H
	JMP FOR

EXIT:
	LXI D, START

PRINT_LOOP:
	LDAX D ; A <- (BC)
	MOV B,A
	MVI A, PRINT_B
	call GTU_OS

	INX D ; go next address
	INX D ; ""
	LDA I ; load current index
	INR A ; increment index
	STA I ; store index
	SUI 26 ; # of items in array(actually memory)
	JNZ PRINT_LOOP ; i -26 != 0 , if not zero go next item


	hlt		; end program
