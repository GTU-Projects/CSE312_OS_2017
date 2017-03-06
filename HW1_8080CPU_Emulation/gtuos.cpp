#include <iostream>
#include <sstream>
#include <string>
#include "8080emuCPP.h"
#include "gtuos.h"


GTUOS::GTUOS() {}

uint64_t GTUOS::handleCall(const CPU8080& cpu8080,int mode){

	std::cout<<"System Call!!"<<std::endl;

    uint8_t  regA = cpu8080.state->a;
    std::cout<<"Register A:"<<unsigned(regA)<<std::endl;

    switch (regA){

        case PRINT_B: printB(cpu8080,mode); break;
        case PRINT_MEM: printB(cpu8080,mode); break;
        case READ_B: printB(cpu8080,mode); break;
        case READ_MEM: printB(cpu8080,mode); break;
        case PRINT_STR: printStr(cpu8080,mode); break;
        case READ_STR: printB(cpu8080,mode); break;

    }

    int b1 = (int)cpu8080.state->b;;
    int c1 = (int)cpu8080.state->b;;

	return 0;
}


void GTUOS::printB(const CPU8080& cpu8080,int mode){
    std::cout<<"Content of B: "<<unsigned(cpu8080.state->b)<<std::endl;
}


//TODO: b ve c de 9dan büyük değerlerin hexe cevirilip adres araması yapılacak ve sorun cozulecek
void GTUOS::printStr(const CPU8080 &cpu8080, int mode) {
    char addr[8];

    sprintf(addr,"0x%d%d",cpu8080.state->b,cpu8080.state->c);

    std::string str(addr);

    std::istringstream buff(addr);

    std::cout<<"Address:"<<str<<std::endl;

    uint64_t val;

    buff >> std::hex >> val;

    std::cout<< "Val:"<<val<<std::endl;//cpu8080.memory[]

    std::cout<<"X:"<<cpu8080.memory[val];

    /*int i=0;
    while(true){
        if(cpu8080.memory[0]==48){
            break;
        }
        ++i;
        std::cout<<i<<std::endl;
    }*/




}
