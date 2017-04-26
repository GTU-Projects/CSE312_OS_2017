#ifndef H_GTUOS
#define H_GTUOS

#include <string>
#include "8080emuCPP.h"
#include "memory.h"


#define MAX_PROC_NAME 255
#define MAX_PROC_COUNT 4
#define CS_CYCLE 100 // context switch cycle

// log settings
#define LOG_FD stdout
#define DEBUG 1

// for colored texts
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

using namespace std;


typedef enum {
    PRINT_B = 1,
    PRINT_MEM = 2,
    READ_B = 3,
    READ_MEM = 4,
    PRINT_STR = 5,
    READ_STR = 6,
    FORK = 7,
    EXEC = 8,
    WAITPID = 9
} SystemCallType;

typedef enum {
    READY = 0,
    BLOCKED = 1,
    RUNNING = 2
} ProcessState;

typedef struct {
    char name[MAX_PROC_NAME];
    State8080 state8080;    // saved registers like a,b,c,sp,pc
    uint64_t baseReg;
    uint64_t limitReg;
    uint16_t pid;
    uint16_t ppid;  // parent pid
    uint64_t startTime; // starting time of process, the cycle number of the CPU
    uint64_t cycle; // how many cycle the process hadd used so far
    ProcessState  procState; // ready, blocked, running
    uint64_t address;   //physical addres of the memory location of process
    uint8_t isAlive;
    uint8_t childNum;
} ProcessInfo;

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
    GTUOS(CPU8080* cpu8080);
    uint64_t handleCall();
    void saveMemoryContents(const string& filename);
    void saveProcInfos(const string& filename);

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
