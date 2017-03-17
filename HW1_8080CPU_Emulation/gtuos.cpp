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
        case READ_MEM: cycleTime = readMem(cpu8080); break;
        case PRINT_STR: cycleTime = printStr(cpu8080); break;
        case READ_STR: cycleTime = readStr(cpu8080); break;
        default: std::cerr<<"Invalid system call"<<std::endl; exit(EXIT_FAILURE);
    }

	return cycleTime;
}

/**
 * Prints content of register b
 * @param cpu8080
 * @return cycle time of print b operation
 */
int GTUOS::printB(const CPU8080& cpu8080){
    std::cout<<"System Call PRINT_B"<<std::endl;
    std::cout<<"Value of B(decimal): "<<int(cpu8080.state->b)<<std::endl;
    return PRINT_B_CYCLE;
}


int GTUOS::readB(const CPU8080 &cpu8080) {
    int b;
    std::cout<<"System Call READ_B"<<std::endl;


    std::cout<<"Enter an integer(0-255) for reg B:";
    std::cin>>b;

    if(b<0 || b>255){
        std::cout<<"uint8t -> Bound error. Assigned zero to b register"<<std::endl;
        cpu8080.state->b=0;
    }else
        cpu8080.state->b = b;

    return READ_B_CYCLE;
}

/**
 * Prints string which address stored in reg[bc]
 * @param cpu8080
 * @return cycle time of printstr
 */
int GTUOS::printStr(const CPU8080 &cpu8080) {
    std::cout<<"System Call PRINT_STR"<<std::endl;

    uint16_t address = (cpu8080.state->b << 8) | cpu8080.state->c;
    address = (cpu8080.state->b << 8) | cpu8080.state->c;

    /*another way to obtain hex address from register bc*/
    /*
    char addr[16];
    sprintf(addr,"0x%x%x",cpu8080.state->b,cpu8080.state->c);
    std::cout<<"Content of Register (address)BC: "<<addr<<std::endl;

    std::string str(addr);
    std::istringstream buff(addr);
    uint64_t val;

    buff >> std::hex >> val; // string to hex
    */

    while(true){
        if(cpu8080.memory[address]=='\0'){
            break;
        }
        std::cout<<cpu8080.memory[address];
        ++address;
    }

    return PRINT_STR_CYCLE;
}

int GTUOS::readStr(const CPU8080 &cpu8080) {
    std::string str;
    int i;
    uint16_t address;
    std::cout<<"System Call READ_STR"<<std::endl;

    getline(std::cin,str);

    address = (cpu8080.state->b << 8) | cpu8080.state->c;

    for(i=0;i<str.length();++i){
        cpu8080.memory[address+i] = str[i];
    }
    cpu8080.memory[address+i]='\0';

    std::cout<<"String writed on address:"<<unsigned(address)<<std::endl;
    return READ_STR_CYCLE;
}



int GTUOS::printMem(const CPU8080& cpu8080){
    char addr[16];
    uint16_t address;
    std::cout<<"System Call PRINT_MEM"<<std::endl;

    address = (cpu8080.state->b << 8) | cpu8080.state->c;
    std::cout<<"Content of Register (address)BC: "<<address<<std::endl;

    std::cout<<"Memory content which pointed with BC:"<<(int)cpu8080.memory[address]<<std::endl;

    return PRINT_MEM_CYCLE;
}


int GTUOS::readMem(const CPU8080 &cpu8080) {

    int number;
    uint16_t address;
    std::cout<<"System Call READ_MEM"<<std::endl;
    std::cout<<"Enter an integer(0-255) for MEM(BC)=:";
    std::cin>>number;

    address = (cpu8080.state->b << 8) | cpu8080.state->c;

    if(number<0 || number>255){
        std::cout<<"uint8t -> Bound error. Assigned zero to MEM(BC)"<<std::endl;
        cpu8080.memory[address]=0;
    }else
        cpu8080.memory[address] = number;

    std::cout<<"Integer stored on memory[BC]:"<<unsigned(address)<<std::endl;
}




void GTUOS::saveMemoryContents(std::ofstream& output,const CPU8080& cpu){

    for(int i=0;i<0x1000;++i){
        char str[5];
        sprintf(str,"%04x",i*16);
        output<<str<<" ";
        for(int j=0;j<0x10;++j) {
            sprintf(str,"%02x",cpu.memory[i*16+j]);
            output << str << " ";
        }
        output<<std::endl;
    }
}

void GTUOS::test(const CPU8080& cpu){

    char addr[10];
    cpu.state->b=0x00;
    cpu.state->c=0x40;
    readMem(cpu);
    printMem(cpu);

    sprintf(addr,"0x%x%x",cpu.state->b,cpu.state->c);
    std::cout<<addr;
    readStr(cpu);
    printStr(cpu);
}

