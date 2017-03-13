#include <iostream>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: prog exeFile debugOption\n";
        exit(1);
    }
    int DEBUG = atoi(argv[2]);
    uint64_t totalCycleTimes = 0;

    std::ofstream output;
    std::string filename = "exe.mem";

    CPU8080 theCPU;
    GTUOS theOS;

    theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);

    output.open(filename.c_str());

    if (!output.is_open()) {
        std::cerr << "Unable to open output file:" << filename << std::endl;
        std::cerr << "Writing memory status is failed" << std::endl;
        return 1;
    }

    do {
        totalCycleTimes += theCPU.Emulate8080p(DEBUG);

        if (theCPU.isSystemCall())
            totalCycleTimes += theOS.handleCall(theCPU);

        if (DEBUG == 2)
            std::cin.get();

    } while (!theCPU.isHalted());

    std::cout << "Total Number of Cycles :" << totalCycleTimes << std::endl;

    theOS.saveMemoryContents(output, theCPU);
    output.close();

    return 0;
}

