#ifndef MMU_HM
#define MMU_HM

#include <cstdlib>
#include "memoryBase.h"

class MMU_HM: public MemoryBase {
public:
	MMU_HM(uint64_t size);
	~MMU_HM() { free(mem);}
	virtual uint8_t & at(uint32_t ind);
	virtual uint8_t & physicalAt(uint32_t ind) { return mem[ind];}
	uint16_t getBaseRegister() const { return baseRegister;}
	uint16_t getLimitRegister() const { return limitRegister;}
	void setBaseResister(uint16_t base) { this->baseRegister = base;}
	void setLimitRegister(uint16_t limit) {this->limitRegister = limit;}

private:
	uint8_t * mem;
	uint16_t baseRegister;
	uint16_t limitRegister;
};



#endif