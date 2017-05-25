        ; 8080 assembler code
        .hexfile parallel.hex
        .binfile parallel.com
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
stack   equ 0F00H

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
	pop H
	pop D
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE

	; my variables to use in searching
	ORG 0010H
bye_msg:	DW '##-->good bye',00AH,00H
out_hello:	DW '##-->Welcome parallel programming example',00AH,00H
wait_pids:	DW 0,0,0
fork_num:	DW 3
fork_num1:	DW 3
proc1:		DW 'PrintNumbers.com',00H
proc2:		DW 'PrintNumbersRev.com',00H
proc3:		DW 'sum.com',00H



	ORG 0100H

begin:
	LXI SP,stack 	; always initialize the stack pointer

	LXI B,out_hello
	MVI A, PRINT_STR ; store system call type
	call GTU_OS ; system call

	LXI D, wait_pids

FORK_AGAIN:
	MVI A, FORK ; store system call type
	call GTU_OS ; system call

	MVI A, 1 ; load 1 to reg a, for error check
	CMP B ; if returned pid is 1, there is a fork error
	JZ ERROR_EXIT 

	MVI A, 0 ; 
	CMP B ; if pid==0 go child area, else parent area
	JZ CHILD_AREA

	MOV A,B
	STAX D
	INX D
	INX D
	
	LDA fork_num
	DCR A
	STA fork_num
	JNZ FORK_AGAIN ; 3 times fork 

	JMP PARENT_AREA

PARENT_AREA:

	LXI D,wait_pids 

WAIT_LOOP:
	LDAX D ;sıradaki pid al
	INX D
	INX D
	MOV B,A ; b ye yukle
	MVI A, WAITPID ; wait until child finish
	call GTU_OS ; system call

	LDA fork_num1  ; tum cocukları wait edene kadar bekle
	DCR A
	STA fork_num1
	JNZ WAIT_LOOP 
	JMP END

CHILD_AREA:

	LDA fork_num // 3 ise p1 yükle
	MOV B, A
	MVI A, 3
	SUB B
	JZ LOAD_P3
	
	LDA fork_num // 2. proc ise p2 yukle
	MOV B, A
	MVI A, 2
	SUB B
	JZ LOAD_P2

	LDA fork_num // 1.proc ise p1 yukle
	MOV B, A
	MVI A, 1
	SUB B
	JZ LOAD_P1
	JMP END


LOAD_P1:
	LXI B,proc1
	MVI A, EXEC ; call execve to run new program
	call GTU_OS ; system call
	JMP END
LOAD_P2:
	LXI B,proc2
	MVI A, EXEC ; call execve to run new program
	call GTU_OS ; system call
	JMP END
LOAD_P3:
	LXI B,proc3
	MVI A, EXEC ; call execve to run new program
	call GTU_OS ; system call
	JMP END

ERROR_EXIT

END:
	hlt    ; end program