#include <iostream>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"

	// This is just a sample main function, you should rewrite this file to handle problems
	// with new multitasking and virtual memory additions.
int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1);
	}
	int DEBUG = atoi(argv[2]);
	uint64_t totalCycleTimes = 0;

	Memory mem(0x1000000); // create 64K memory
	CPU8080 theCPU(&mem);
	GTUOS	theOS(&theCPU);
	

	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000); // initialize memory

	theOS.setDebugMode(DEBUG);
	totalCycleTimes = theOS.run();
	std::cout << "Total Number of Cycles :" << totalCycleTimes << std::endl;

	
	theOS.saveMemoryContents("exe.mem");
	
	return 0;
}

