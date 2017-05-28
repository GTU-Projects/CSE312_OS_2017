#include <iostream>
#include "MMU_HM.h"


MMU_HM::MMU_HM(uint64_t memSize, uint64_t diskSize) {
	mem = (uint8_t*) calloc(memSize, sizeof(uint8_t));
	disk = (uint8_t*) calloc(diskSize, sizeof(uint8_t));
	baseRegister = 0;
	limitRegister = 0;
}

MMU_HM::~MMU_HM() {
	free(disk);
	free(mem);
}

uint8_t & MMU_HM::at(uint32_t ind) {
	return mem[ind + baseRegister];
}

uint8_t & MMU_HM::physicalAt(uint32_t ind) {
	return disk[ind];
}

void MMU_HM::initMM(){

	for(int i=0;i<SIZE1K;++i){
		mem[i]=disk[i];
	}
	limitRegister=SIZE8K;
}



