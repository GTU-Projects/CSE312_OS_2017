#ifndef H_MEMORY
#define H_MEMORY

#include <cstdlib>
#include "memoryBase.h"

// This is just a simple memory with no virtual addresses.
// You will write your own memory with base and limit registers.

class memory: public MemoryBase {
public:
    memory();
		~memory(){ free(mem);}
		virtual uint8_t & at(uint32_t ind) { return  mem[ind];}
		virtual uint8_t & physicalAt(uint32_t ind) { return mem[ind];}
private:
		uint8_t * mem;
    uint16_t baseRegister;
    uint16_t limitRegister;
};

#endif


