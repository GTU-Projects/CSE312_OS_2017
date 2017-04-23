SOURCE_FILES=main.cpp gtuos.cpp 8080emu.cpp memory.cpp
HEADER_FILES=gtuos.h memory.h memoryBase.h 8080emuCPP.h

FLAGS=

all: compile run

compile: clean
	g++ ${SOURCE_FILES} ${HEADER_FILES} ${FLAGS} -o exe

clean:
	rm -rf *.o exe

move:
	mv ~/Downloads/shell.com ./asm/

run:	
	./exe ./asm/shell.com 0