SOURCE_FILES=main.cpp gtuos.cpp 8080emu.cpp memory.cpp
HEADER_FILES=gtuos.h memory.h memoryBase.h 8080emuCPP.h

FLAGS= #-std=c++11
DIRECIVES= #-DDEBUG


all: clean compile

compile: 
	g++ ${HEADER_FILES} ${SOURCE_FILES}  ${FLAGS} ${DIRECIVES} -o exe

clean:
	rm -rf *.o exe

move:
	mv ~/Downloads/shell.com ./asm/
move1:
	mv ~/Downloads/parallel.com ./asm/

eMove:
	mv ~/Downloads/exec.com ./asm/
run:	
	./exe shell.com 0

run2:
	./exe parallel.com 2
