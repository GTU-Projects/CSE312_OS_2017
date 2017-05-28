#include <iostream>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "MMU_HM.h"

int main (int argc, char**argv)
{
	if (argc != 3) {
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1);
	}
	int osDebugMode = atoi(argv[2]);
	uint64_t totalCycleTimes = 0;

	MMU_HM mem(0x1000, 0x1000000); // 8K mem - 64K disk
	CPU8080 theCPU(&mem);
	GTUOS	theOS(&theCPU, argv[1]);


	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000); // initialize memory
	mem.initMM(); // load 1KB data from disk to 

	theOS.setDebugMode(osDebugMode);
	totalCycleTimes = theOS.run();
	std::cout << BLU << "Total Number of Cycles :" << totalCycleTimes << RESET << std::endl;

	theOS.saveMemoryContents("exe.mem");

	// PARALLEL ASM calıstıgında proces bilgileri buraya da eklenir
	theOS.saveProcInfos("proc_list.txt");
	

	return 0;
}

