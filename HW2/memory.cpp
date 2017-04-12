#include <iostream>
#include "memory.h"

memory::memory(){
  mem = (uint8_t*) malloc(0x10000);
  baseRegister=0;
  limitRegister=0;
}