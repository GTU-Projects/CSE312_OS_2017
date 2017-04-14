SOURCE_FILES=main.cpp gtuos.cpp 8080emu.cpp memory.cpp
HEADER_FILES=gtuos.h memory.h memoryBase.h 8080emuCPP.h

FLAGS=-std=c++11

all: compile

compile: clean
	g++ ${SOURCE_FILES} ${HEADER_FILES} ${FLAGS} -o exe

clean:
	rm -rf *.o exe