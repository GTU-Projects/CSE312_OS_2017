#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "8080emuCPP.h"
#include "gtuos.h"



GTUOS::GTUOS() {
}

uint64_t GTUOS::handleCall(const CPU8080& cpu8080){

    uint8_t  regA = cpu8080.state->a;
    int cycleTime=0;

    //std::cout<<"Value of regA:"<<unsigned(regA)<<std::endl;

    switch (regA){
        case PRINT_B: cycleTime = printB(cpu8080); break;
        case PRINT_MEM: cycleTime = printMem(cpu8080); break;
        case READ_B: cycleTime = readB(cpu8080); break;
        case READ_MEM: cycleTime = printB(cpu8080); break;
        case PRINT_STR: cycleTime = printStr(cpu8080); break;
        case READ_STR: cycleTime = printB(cpu8080); break;
        default: std::cerr<<"Invalid system call"<<std::endl; exit(EXIT_FAILURE);
    }

	return cycleTime;
}


int GTUOS::printB(const CPU8080& cpu8080){
    std::cout<<"System Call PRINT_B"<<std::endl;
    std::cout<<"Value of B(decimal): "<<int(cpu8080.state->b)<<std::endl;
    return PRINT_B_CYCLE;
}

int GTUOS::printStr(const CPU8080 &cpu8080) {
    char addr[16];
    std::cout<<"System Call PRINT_STR"<<std::endl;

    sprintf(addr,"0x%x%x",cpu8080.state->b,cpu8080.state->c);
    std::cout<<"Content of Register (address)BC: "<<addr<<std::endl;

    std::string str(addr);
    std::istringstream buff(addr);
    uint64_t val;

    buff >> std::hex >> val;

    while(true){
        if(cpu8080.memory[val]=='\0'){
            break;
        }
        std::cout<<cpu8080.memory[val];
        ++val;
    }

    return 100;
}

int GTUOS::printMem(const CPU8080& cpu8080){
    char addr[16];
    std::cout<<"System Call PRINT_MEM"<<std::endl;

    sprintf(addr,"0x%x%x",cpu8080.state->b,cpu8080.state->c);
    std::cout<<"Content of Register (address)BC: "<<addr<<std::endl;

    std::string str(addr);
    std::istringstream buff(addr);
    uint64_t val;

    buff >> std::hex >> val;

    std::cout<<"Memory content which pointed with BC:"<<cpu8080.memory[val]<<std::endl;

    return PRINT_MEM_CYCLE;
}

int GTUOS::readB(const CPU8080 &cpu8080) {
    int b;
    std::cout<<"System Call READ_B"<<std::endl;

    std::cout<<"Enter an integer(0-255) for reg B:";
    std::cin>>b;
    if(b<0 || b>255)
        cpu8080.state->b=0;
    else
        cpu8080.state->b = b;

    char addr[16];
    sprintf(addr,"0x%x",cpu8080.state->b);
    std::cout<<"addr"<<addr;

    return READ_B_CYCLE;
}

void GTUOS::saveMemoryContents(std::ofstream& output,const CPU8080& cpu){

    for(int i=0;i<0x10000;++i){
        char str[5];
        sprintf(str,"%x %x\n",i,cpu.memory[i]);
        output<<str;
    }
}

