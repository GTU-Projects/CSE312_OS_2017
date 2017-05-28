#ifndef GENERAL_TYPES
#define GENERAL_TYPES

#include "memoryBase.h"
#include "8080emuCPP.h"

#define MAX_PROC_NAME 255
#define MAX_PROC_COUNT 4
#define CS_CYCLE 100 // context switch cycle
#define MAX_PATH_LEN 255

// log settings
#define LOG_FD stdout
//#define DEBUG 1

// for colored texts
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

// paging values
#define SIZE1K 1024
#define SIZE8K 8192
#define PAGE_TABLE_SIZE 16 // 16*1KB = 16KB Virtual Address Space

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


/*
	VA = Page(4bit -> 2^4 : 16 PageNum) + Offset(10bit -> 2^10 : 1KB PageSize)
	PA = Frame(3bit -> 2^3: 8 PageNum for mem) + Offset(10bit -> 2^10 : 1KB PageSize)
*/

typedef struct {
	uint8_t modifiedBit;
	uint8_t referencedBit;
	uint8_t present;
	uint8_t frame; //
} Page_t;


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
	uint16_t waitIndex;

	Page_t pageTable[PAGE_TABLE_SIZE]; // 16KB virtual space

} ProcessInfo;





#endif