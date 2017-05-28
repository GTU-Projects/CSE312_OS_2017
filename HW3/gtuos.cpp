#include <iostream>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "MMU_HM.h"

#define TEST_ASM "./sum.com"

GTUOS::GTUOS(CPU8080* cpu8080, const char *initialName) {

  theCPU = cpu8080;
  processTable = (ProcessInfo *)calloc(sizeof(ProcessInfo), MAX_PROC_COUNT);
  // calloc already resets the allocated area
  //bzero(processTable,MAX_PROC_COUNT*sizeof(ProcessInfo));
  strcpy(processTable[0].name, initialName);
  processTable[0].state8080 = *(theCPU->state);
  processTable[0].baseReg = 0;
  processTable[0].limitReg = 0x10000 - 1; // can access all 64K
  processTable[0].pid = 2;
  processTable[0].ppid = 2; // Parent of os is os
  processTable[0].startTime = 0;
  processTable[0].cycle = 0;
  processTable[0].procState = RUNNING;
  processTable[0].address = 0;
  processTable[0].isAlive = 1;
  processTable[0].waitIndex = 0;

  debugMode = 0;
  currProcInd = 0;



}

bool GTUOS::isAllProcessesDone()const {
  for (uint8_t i = 0; i < MAX_PROC_COUNT; ++i) {
    if (processTable[i].isAlive)
      return false;
  }
  return true;
}

uint64_t GTUOS::run() {

  uint64_t totalCycleTimes = 0;
  uint16_t csCycle = 0; //context switch cycle
  uint16_t cycle = 0;
  uint16_t t;
  uint16_t nextIndex;
  bool makeSwitch = false;

  do {
    if (debugMode == 2)
      std::cin.get();
    ProcessState st = processTable[currProcInd].procState;


#ifdef DEBUG
    printf("Process Index:%d\n", currProcInd);
    printf("State:%d\n", st );
#endif

    t = theCPU->Emulate8080p(debugMode);


    if ( theCPU->isSystemCall()) { // isHalted()
      t += this->handleCall();
    }

    cycle += t;
    totalCycleTimes += t;

    if (((cycle / CS_CYCLE) > 0 || st == BLOCKED ) && !theCPU->isHalted()) { // round robin context switch cycle
      cycle = cycle % CS_CYCLE;
      nextIndex = getNextProcInd();
      if (nextIndex != currProcInd) // no need if same proc
        contextSwitch(currProcInd, nextIndex);
      continue;
    }

#ifdef DEBUG
    printf("pc:%x - lastopcode:%x\n", theCPU->state->pc, *(theCPU->lastOpcode ));
#endif
    if (theCPU->isHalted()) { // is Halted operation
      processTable[currProcInd].isAlive = false; // process dead
      // send signal to parent
      if (currProcInd != 0) {
        processTable[processTable[currProcInd].ppid - 2].procState = READY;
      }
      printf(RED "Process:%d dead\n" RESET , currProcInd );
      nextIndex = getNextProcInd();
      if (nextIndex != currProcInd)
        contextSwitch(currProcInd, nextIndex);
    }
    st = processTable[currProcInd].procState;

    if ((cycle / CS_CYCLE) > 0 || st == BLOCKED) { // round robin context switch cycle
      cycle = cycle % CS_CYCLE;
      nextIndex = getNextProcInd();
      if (nextIndex != currProcInd) // no need if same proc
        contextSwitch(currProcInd, nextIndex);
      continue;
    }

  } while (!isAllProcessesDone());


  return totalCycleTimes;
}

// p1 -> p2
void GTUOS::contextSwitch(uint8_t p1, uint8_t p2) {

  if (debugMode == 2 || debugMode == 3)
    std::cout << BLU << "-->Context switch occur between " << processTable[p1].name << "-->" << processTable[p2].name << RESET << std::endl;

  processTable[p1].state8080 = *(theCPU->state);
  if (processTable[p1].procState != BLOCKED) // bloklanmadıysa, devam
    processTable[p1].procState = READY;


  *(theCPU->state) = processTable[p2].state8080;
  processTable[p2].procState = RUNNING;

  Memory *mem = (Memory*)theCPU->memory;
  mem->setBaseResister(processTable[p2].baseReg);
  mem->setLimitRegister(processTable[p2].limitReg);

  currProcInd = p2; // switch current process index
}

uint8_t GTUOS::getNextProcInd() const {
  for (uint8_t i = currProcInd + 1; i < MAX_PROC_COUNT; ++i)
    if (processTable[i].isAlive && processTable[i].procState == READY) // get next proc which is live
      return i;

  // if there is not alive process forward locations , look backwards
  for (uint8_t i = 0; i < currProcInd; ++i)
    if (processTable[i].isAlive && processTable[i].procState == READY)
      return i;

  // there is just one process
  return currProcInd;
}


uint64_t GTUOS::handleCall() {

  uint8_t regA = theCPU->state->a;
  int cycleTime = 0;

  std::cout<<"Value of regA:"<<unsigned(regA)<<std::endl;

  switch (regA) {
  case PRINT_B:
    cycleTime = printB();
    break;
  case PRINT_MEM:
    cycleTime = printMem();
    break;
  case READ_B:
    cycleTime = readB();
    break;
  case READ_MEM:
    cycleTime = readMem();
    break;
  case PRINT_STR:
    cycleTime = printStr();
    break;
  case READ_STR:
    cycleTime = readStr();
    break;
  case FORK:
    cycleTime = this->fork();
    break;
  case EXEC:
    cycleTime = this->exec();
    break;
  case WAITPID:
    cycleTime = this->waitpid();
    break;
  default:
    std::cerr << "Invalid system call" << (int)regA << std::endl;
    exit(EXIT_FAILURE);
  }

  return cycleTime;
}

/**
 * Prints content of register b
 * @param cpu8080
 * @return cycle time of print b operation
 */
uint8_t GTUOS::printB() {
  std::cout << GRN << "SystemCall: PRINT_B" << RESET << std::endl;
  std::cout << "Value of B(decimal): " << int(theCPU->state->b) << std::endl;
  return CycleTime::PRINT_B;
}


uint8_t GTUOS::readB() {
  int b;
  std::cout << "SystemCall: READ_B" << std::endl;

  std::cout << "Enter an integer(0-255) for reg B:";
  scanf("%d", &b);

  printf(RED "B:%d\n" RESET, b );

  if (b < 0 || b > 255) {
    std::cout << "uint8t -> Bound error. Assigned zero to b register" << std::endl;
    theCPU->state->b = 0;
  } else
    theCPU->state->b = b;

  return CycleTime::READ_B;
}

/**
 * Prints string which address stored in reg[bc]
 * @param cpu8080
 * @return cycle time of printstr
 */
uint8_t GTUOS::printStr() {
  std::cout << GRN << "SystemCall: PRINT_STR" << RESET << std::endl;

  uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;

  while (true) {
    if (theCPU->memory->at(address) == '\0') {
      break;
    }
    std::cout << theCPU->memory->at(address);
    ++address;
  }

  return CycleTime::PRINT_STR;
}

uint8_t GTUOS::readStr() {
  char str[255];
  int i;
  uint16_t address;
  std::cout << GRN << "SystemCall: READ_STR" << RESET << std::endl;
  std::cout << "Enter a string: ";

  scanf(" %[^\n]s", str); // updated for junk whitespaces

  printf(RED "-%s-\n" RESET, str );

  address = (theCPU->state->b << 8) | theCPU->state->c;

  for (i = 0; i < strlen(str); ++i) {
    theCPU->memory->at(address + i) = str[i];
  }
  theCPU->memory->at(address + i) = '\0';

  std::cout << "String writed on address:" << unsigned(address) << std::endl;
  return CycleTime::READ_STR;
}


uint8_t GTUOS::printMem() {
  char addr[16];
  uint16_t address;
  std::cout << GRN << "SystemCall: PRINT_MEM" << RESET << std::endl;

  address = (theCPU->state->b << 8) | theCPU->state->c;
  std::cout << "Content of Register (address)BC: " << address << std::endl;

  std::cout << "Memory content which pointed with BC:" << (int) theCPU->memory->at(address) << std::endl;

  return CycleTime::PRINT_MEM;
}


uint8_t GTUOS::readMem() {
  int number;
  uint16_t address;
  std::cout << GRN << "SystemCall: READ_MEM" << RESET << std::endl;
  std::cout << "Enter an integer(0-255) for MEM(BC)=:";
  std::cin >> number;

  address = (theCPU->state->b << 8) | theCPU->state->c;

  if (number < 0 || number > 255) {
    std::cout << "uint8t -> Bound error. Assigned zero to MEM(BC)" << std::endl;
    theCPU->memory->at(address) = 0;
  } else
    theCPU->memory->at(address) = number;

  std::cout << "Integer stored on memory[BC]:" << unsigned(address) << std::endl;
  return CycleTime::READ_MEM;
}


void GTUOS::test(const CPU8080 &cpu) {

  char addr[10];
  cpu.state->b = 0x00;
  cpu.state->c = 0x40;
  readMem();
  printMem();

  sprintf(addr, "0x%x%x", cpu.state->b, cpu.state->c);
  std::cout << addr;
  readStr();
  printStr();
}

// switch cpu state with p1 process
void GTUOS::copyCurrProcState(uint8_t p1) {
  // copy all needed datas
  processTable[p1].state8080 = *(theCPU->state); // copy current state

  processTable[p1].state8080.cc = theCPU->state->cc; // copy condition codes
  //printf("%d-%d", processTable[p1].state8080.cc.cy, theCPU->state->cc.cy);

  strcat(processTable[p1].name, "_clone");
  processTable[p1].baseReg = 0x4000 * p1; // 4KB per process
  processTable[p1].limitReg = 0x4000 * (p1 + 1) - 1;
  processTable[p1].pid = p1 + 2;
  processTable[p1].ppid = processTable[currProcInd].pid;
  processTable[p1].procState = READY;
  processTable[p1].address = processTable[p1].baseReg;
  processTable[p1].isAlive = 1;

  // prepare returned pid values
  processTable[p1].state8080.b = 0; // return 0 to child

#ifdef DEBUG
  fprintf(LOG_FD, "Current Process state copied into processTable[%d]\n", p1);
#endif

}

uint8_t GTUOS::fork() {

  printf(GRN "SystemCall: FORK\n" RESET);

  uint8_t nextEmpty = -1; // next empty area for process
  // get nex empty location
  for (uint8_t i = 0; i < MAX_PROC_COUNT; ++i) {
    if (i != currProcInd && !processTable[i].isAlive) {
      nextEmpty = i;
      break;
    }
  }

  if (nextEmpty == -1) { // memory full or other errors ...
    theCPU->state->b = 1;
    return nextEmpty;
  }

  copyCurrProcState(nextEmpty);

  theCPU->state->b = processTable[nextEmpty].pid; // return child pid to parent

  // copy parent memory to child
  copyMemory(processTable[currProcInd].address, processTable[nextEmpty].address);

  // print process informations for test
#ifdef DEBUG
  printProcInfs(nextEmpty);
#endif

  return CycleTime::FORK;
}

uint8_t GTUOS::exec() {
  char filename[MAX_PATH_LEN];
  int i = 0;
  printf(GRN "SystemCall: EXEC\n" RESET );

  bzero(filename, MAX_PATH_LEN);

  uint16_t address = (theCPU->state->b << 8) | theCPU->state->c;

  while (true) {
    if (theCPU->memory->at(address) == '\0') {
      break;
    }
    filename[i] = theCPU->memory->at(address);
    ++i;
    ++address;
  }

  filename[i] = '\0';

  if (strcmp(filename, "exit()") == 0) {
    printf(RED "EXIT FORCED\n" RESET);
    exit(1);
  }
  // read new program into memory

  strcpy(processTable[currProcInd].name, filename); // set process name

  theCPU->ReadFileIntoMemoryAt(filename, ((Memory*)(theCPU->memory))->getBaseRegister());
  bzero(theCPU->state, sizeof(State8080)); // reset registers

  return CycleTime::EXEC;
}

uint8_t GTUOS::waitpid() {

  uint8_t i = 0, index = 0;
  printf(GRN "SystemCall: WAITPID\n" RESET);

  // wait process index
  uint8_t b = theCPU->state->b; // get wait pid
  for (i = 0; i < MAX_PROC_COUNT; ++i)
    if (processTable[i].isAlive && processTable[i].pid == b) {
      printf("Parent will wait for pid:%d\n", processTable[i].pid);
      processTable[currProcInd].procState = BLOCKED;
      return 0; // wait time
    }

  printf("wait complete : %d\n", b );

  processTable[currProcInd].procState = READY;
  return CycleTime::WAITPID;
}

void GTUOS::setDebugMode(uint8_t mode) {
  debugMode = mode;
}

void GTUOS::printProcInfs(uint64_t ind) const {
  printf("Process Name: %s\n", processTable[ind].name);
  printf("Pid: %d - PPid:%d\n", processTable[ind].pid, processTable[ind].ppid );
  printf("Address: %ld\n", processTable[ind].address );
}

// copy 16K memory from x to y
void GTUOS::copyMemory(uint64_t f, uint64_t t) {

  for (uint32_t i = 0; i < 0x4000; ++i) {
    theCPU->memory->physicalAt(t + i) = theCPU->memory->physicalAt(f + i);
  }

}


void GTUOS::saveMemoryContents(const string& filename) {
  std::ofstream output;

  output.open(filename.c_str());
  if (!output.is_open()) {
    std::cerr << "Unable to open output file:" << filename << std::endl;
    std::cerr << "Writing memory status is failed" << std::endl;
    return;
  }

  for (int i = 0; i < 0x1000; ++i) {
    char str[5];
    sprintf(str, "%04x", i * 16);
    output << str << " ";
    for (int j = 0; j < 0x10; ++j) {
      sprintf(str, "%02x", theCPU->memory->physicalAt(i * 16 + j));
      output << str << " ";
    }
    output << std::endl;
  }

  output.close();

}


void GTUOS::saveProcInfos(const string& filename) {

  std::ofstream output;

  output.open(filename.c_str());
  if (!output.is_open()) {
    std::cerr << "Unable to open output file:" << filename << std::endl;
    std::cerr << "Writing processes infos is failed" << std::endl;
    return;
  }

  for (int i = 0; i < MAX_PROC_COUNT; ++i) {
    output << "--------------------------------------" << endl;
    output << "PID:" << processTable[i].pid << ", PPID:" << processTable[i].ppid << ", Name:" << processTable[i].name << endl;
    output << "Base Register:" << processTable[i].baseReg << ", Limit Register:" << processTable[i].limitReg << endl;
  }

  output.close();


}
