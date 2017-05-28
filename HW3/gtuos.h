#ifndef H_GTUOS
#define H_GTUOS

#include <string>
#include "8080emuCPP.h"
#include "memory.h"
#include "MMU_HM.h"
#include "general_types.h"


class CycleTime {
public:
    CycleTime();
    const static uint8_t PRINT_B = 10;
    const static uint8_t PRINT_MEM = 10;
    const static uint8_t READ_B = 10;
    const static uint8_t READ_MEM = 10;
    const static uint8_t PRINT_STR = 100;
    const static uint8_t READ_STR = 100;
    const static uint8_t FORK = 50;
    const static uint8_t EXEC = 80;
    const static uint8_t WAITPID = 80;
};


class GTUOS {

public:
    GTUOS(CPU8080* cpu8080,const char *initialName);
    uint64_t handleCall();
    void saveMemoryContents(const string& filename);
    void saveProcInfos(const string& filename);

    ProcessInfo* getCurrentProcessInfo(){
        return &processTable[currProcInd];
    }

    void setDebugMode(uint8_t mode);
    uint64_t run();
    void test(const CPU8080& cpu);

private:

    ProcessInfo *processTable ;

    CPU8080 *theCPU;
    uint8_t debugMode;
    uint8_t currProcInd;

    uint8_t printB();
    uint8_t printStr();
    uint8_t printMem();
    uint8_t readB();
    uint8_t readMem();
    uint8_t readStr();
    uint8_t fork();
    uint8_t exec();
    uint8_t waitpid();

    void printProcInfs(uint64_t ind) const;

    uint8_t getNextProcInd() const;
    bool isAllProcessesDone() const;


    void contextSwitch(uint8_t p1, uint8_t p2); // p1 -> p2
    void copyCurrProcState(uint8_t p1);
    void copyMemory(uint64_t f, uint64_t t); // from f to t

};

#endif
