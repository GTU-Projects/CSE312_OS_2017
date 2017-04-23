#include <iostream>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"

GTUOS::GTUOS(CPU8080* cpu8080) {

  theCPU = cpu8080;
  processTable = (ProcessInfo *)calloc(4, sizeof(ProcessInfo));
  sprintf(processTable[0].name, "hmennOS");
  processTable[0].state8080 = *(theCPU->state);
  processTable[0].baseReg = 0;
  processTable[0].limitReg = 0x10000-1; // can access all 64K
  processTable[0].pid = 2;
  processTable[0].ppid = 2; // Parent of os is os
  processTable[0].startTime = 0;
  processTable[0].cycle = 0;
  processTable[0].procState = ProcessState::RUNNING;
  processTable[0].address = 0;
  processTable[0].isFull = 1;

  debugMode = 0;
  currProcInd = 0;

}

uint64_t GTUOS::run() {

  uint64_t totalCycleTimes = 0;


  do {
    totalCycleTimes += theCPU->Emulate8080p(debugMode);
    cout<<RED<<"Cycle:"<<totalCycleTimes<<RESET<<endl;


    if (theCPU->isSystemCall())
      totalCycleTimes += this->handleCall();

    if (debugMode == 2)
      std::cin.get();

  } while (!theCPU->isHalted());

  return totalCycleTimes;
}


uint64_t GTUOS::handleCall() {

  uint8_t regA = theCPU->state->a;
  int cycleTime = 0;

  //std::cout<<"Value of regA:"<<unsigned(regA)<<std::endl;

  switch (regA) {
  case SystemCallType::PRINT_B:
    cycleTime = printB();
    break;
  case SystemCallType::PRINT_MEM:
    cycleTime = printMem();
    break;
  case SystemCallType::READ_B:
    cycleTime = readB();
    break;
  case SystemCallType::READ_MEM:
    cycleTime = readMem();
    break;
  case SystemCallType::PRINT_STR:
    cycleTime = printStr();
    break;
  case SystemCallType::READ_STR:
    cycleTime = readStr();
    break;
  case SystemCallType::FORK:
    cycleTime = this->fork();
    break;
  case SystemCallType::EXEC:
    cycleTime = this->exec();
    break;
  case SystemCallType::WAITPID:
    cycleTime = this->waitpid();
    break;
  default:
    std::cerr << "Invalid system call" << std::endl;
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
  std::cin >> b;

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
  address = (theCPU->state->b << 8) | theCPU->state->c;

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
  std::string str;
  int i;
  uint16_t address;
  std::cout << GRN << "SystemCall: READ_STR" << RESET << std::endl;
  std::cout << "Enter a string: ";
  getline(std::cin, str);

  address = (theCPU->state->b << 8) | theCPU->state->c;

  for (i = 0; i < str.length(); ++i) {
    theCPU->memory->at(address + i) = str[i];
  }
  theCPU->memory->at((address++) + i) = '\n';
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
  std::cout << GRN <<"SystemCall: READ_MEM" << RESET<<std::endl;
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


void GTUOS::saveMemoryContents(const string& filename) {
  std::ofstream output;

  output.open(filename);
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
      sprintf(str, "%02x", theCPU->memory->at(i * 16 + j));
      output << str << " ";
    }
    output << std::endl;
  }

  output.close();

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

uint8_t GTUOS::fork() {

  printf(GRN "SystemCall: FORK\n" RESET);

  int nextEmpty = -1; // next empty area for process
  // get nex empty location
  for (uint8_t i = 0; i < MAX_PROC_COUNT; ++i) {
    if (i != currProcInd && !processTable[i].isFull) {
      nextEmpty = i;
      break;
    }
  }

  if (nextEmpty == -1) { // memory full or other errors ...
    theCPU->state->a = 1;
    return nextEmpty;
  }

  // copy parent informations to child
  processTable[nextEmpty] = processTable[currProcInd];

  strcat(processTable[nextEmpty].name, "_clone");
  processTable[nextEmpty].baseReg = 0x4000 * nextEmpty; // 4KB per process
  processTable[nextEmpty].limitReg = 0x4000 * (nextEmpty + 1) - 1;
  processTable[nextEmpty].pid = nextEmpty + 2;
  processTable[nextEmpty].ppid = processTable[currProcInd].pid;
  processTable[nextEmpty].procState = ProcessState::READY;
  processTable[nextEmpty].address = processTable[nextEmpty].baseReg;
  processTable[nextEmpty].isFull = 1;
  processTable[nextEmpty].state8080.a = processTable[nextEmpty].pid;

  // dupe parent memory to child
  dupeMemory(processTable[currProcInd].address, processTable[nextEmpty].address);

  // print process informations for test
#ifdef DEBUG
  printProcInfs(nextEmpty);
#endif

  return CycleTime::FORK;
}

uint8_t GTUOS::exec() {

}

uint8_t GTUOS::waitpid() {

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
void GTUOS::dupeMemory(uint64_t f, uint64_t t) {

  for (uint32_t i = 0; i < 0x4000; ++i)
    theCPU->memory->physicalAt(t + i) = theCPU->memory->physicalAt(f + i);

}


void GTUOS::saveProcInfos(const string& filename){

  std::ofstream output;

  output.open(filename);
  if (!output.is_open()) {
    std::cerr << "Unable to open output file:" << filename << std::endl;
    std::cerr << "Writing processes infos is failed" << std::endl;
    return;
  }

  for (int i = 0; i < MAX_PROC_COUNT; ++i) {
    output<<"--------------------------------------"<<endl;
    output<< "PID:"<<processTable[i].pid<<", PPID:"<<processTable[i].ppid<<", Name:"<<processTable[i].name<<endl;
    output<< "Base Register:"<<processTable[i].baseReg<<", Limit Register:"<<processTable[i].limitReg<<endl;
  }

  output.close();


}
