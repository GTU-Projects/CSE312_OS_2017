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

in_take_msg:	DW '##-->Enter file name to execute',00AH,00H ; null terminated string
bye_msg:	DW '##-->good bye',00AH,00H
out_hello:	DW '##-->Welcome simple shell program',00AH,00H
parent_msg:	DW '##-->Parent',00AH,00H
child_msg:	DW '##-->Child',00AH,00H
wait_pid:	DB 0

	ORG 0100H

begin:
	LXI SP,stack 	; always initialize the stack pointer

	LXI B,out_hello
	MVI A, PRINT_STR ; store system call type
	call GTU_OS ; system call

LOOP:
	LXI B,in_take_msg
	MVI A, PRINT_STR ; store system call type
	call GTU_OS ; system call

	MVI B, 30H
	MVI C, 00H
	MVI A, READ_STR
	call GTU_OS

	MVI A, FORK ; store system call type
	call GTU_OS ; system call

	MVI A, 1 ; load 1 to reg a, for error chech
	CMP B ; if returned pid is 1, there is a fork error
	JZ ERROR_EXIT 

	MVI A, 0 ; 
	CMP B ; if pid==0 go child area, else parent area
	JZ CHILD_AREA
	JMP PARENT_AREA

PARENT_AREA:
	MVI A, WAITPID ; wait until child finish
	call GTU_OS ; system call
	
	JMP LOOP ; go and take new command

CHILD_AREA:
	MVI B, 30H
	MVI C, 00H
	MVI A, EXEC ; call execve to run new program
	call GTU_OS ; system call
	JMP ERROR_EXIT	
	
	; READ USE AGE AND PRINT SCREEN
	;LXI B,bye_msg ; test for PRINT_STR
	;MVI A, PRINT_STR ; store system call type
	;call GTU_OS ; system call
	;JMP END

ERROR_EXIT:
	

END:
	hlt    ; end program