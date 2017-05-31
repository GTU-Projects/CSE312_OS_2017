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
stack   equ 4000H

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
	ORG 3FEH ;
START:	DW 049H, 0c2H, 08aH, 075H, 025H, 06dH, 0f0H, 0a3H, 09bH, 0bdH, 063H, 0d3H, 037H, 05cH, 0aeH, 00cH, 05cH, 087H, 0eaH, 09eH, 011H, 006H, 0a4H, 096H, 0a2H, 010H, 011H, 0ecH, 008H, 061H, 0dbH, 0d1H, 024H, 066H, 047H, 0c8H, 053H, 0b8H, 06cH, 06eH, 0f5H, 050H, 043H, 02dH, 02cH, 071H, 0b9H, 008H, 078H, 0a4H, 0a6H, 089H, 0aaH, 0cbH, 09fH, 0ccH, 0dbH, 031H, 039H, 063H, 092H, 015H, 035H, 037H, 0faH, 0fcH, 000H, 04fH, 0b5H, 0ecH, 0bdH, 0abH, 0bcH, 001H, 059H, 0e8H, 073H, 013H, 0f0H, 0ebH, 0b7H, 097H, 0f5H, 063H, 0e2H, 015H, 0afH, 03eH, 0c5H, 0e9H, 0a2H, 0d8H, 0feH, 0d7H, 010H, 0faH, 054H, 08fH, 0c9H, 00aH, 0fbH, 087H, 036H, 0b8H, 009H, 08fH, 0a1H, 0fbH, 0a2H, 092H, 0e7H, 0d9H, 0aaH, 05dH, 0bcH, 00dH, 0f2H, 06dH, 04cH, 0b8H, 0d6H, 0eeH, 091H, 0d5H, 046H, 021H, 050H, 09bH, 0b1H, 01aH, 0a5H, 02dH, 0a2H, 05bH, 066H, 02bH, 0eaH, 008H, 027H, 00dH, 01bH, 08eH, 067H, 045H, 0ecH, 024H, 052H, 0dfH, 011H, 09eH, 018H, 067H, 00dH, 0aaH, 03eH, 054H, 04bH, 00eH, 0efH, 0fcH, 029H, 015H, 0aaH, 04bH, 0f0H, 011H, 076H, 05bH, 098H, 09dH, 069H, 0b3H, 02cH, 050H, 0f8H, 098H, 0f3H, 04cH, 0f7H, 085H, 06aH, 011H, 0ecH, 078H, 03bH, 02bH, 0ccH, 086H, 03aH, 03cH, 004H, 0e2H, 0d0H, 02eH, 02eH, 0c1H, 03fH, 024H, 01eH, 0d7H, 041H, 007H, 00cH, 0ecH, 057H, 084H, 086H, 0caH, 050H, 0fdH, 050H, 0bbH, 08eH, 03eH, 0b3H, 049H, 0e8H, 000H, 050H, 0a2H, 03cH, 0d3H, 085H, 08cH, 002H, 033H, 04fH, 0c0H, 0d6H, 0ecH, 098H, 018H, 073H, 024H, 085H, 0caH, 0a9H, 08bH, 015H, 079H, 089H, 066H, 035H, 098H, 0a4H, 0e8H, 0e1H, 00dH, 0e8H, 032H

	ORG 0700H
N:	DB 250 ; number of items to compare
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
	SUI 250 ; # of items in array(actually memory)
	JNZ PRINT_LOOP ; i -250 != 0 , if not zero go next item


	hlt		; end program
