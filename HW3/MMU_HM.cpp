#include <iostream>
#include "MMU_HM.h"

MMU_HM::MMU_HM(uint64_t size) {
	mem = (uint8_t*) calloc(size, sizeof(uint8_t));
	baseRegister = 0;
	limitRegister = 0;

}

uint8_t & MMU_HM::at(uint32_t ind) {
	return mem[ind + baseRegister];
}

