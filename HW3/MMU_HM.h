#ifndef MMU_HM_HEADER
#define MMU_HM_HEADER

#include <cstdlib>
#include "memoryBase.h"
#include "general_types.h"



class MMU_HM: public MemoryBase {
public:
	MMU_HM(uint64_t memSize, uint64_t diskSize);
	~MMU_HM();
	virtual uint8_t & at(uint32_t ind);
	virtual uint8_t & physicalAt(uint32_t ind);
	uint16_t getBaseRegister() const { return baseRegister;}
	uint16_t getLimitRegister() const { return limitRegister;}
	void setBaseResister(uint16_t base) { this->baseRegister = base;}
	void setLimitRegister(uint16_t limit) {this->limitRegister = limit;}

	void initMM();

	void setProcessInfo(ProcessInfo *processInfo);
	void loadPageAt(int memPageIndex, int diskPageIndex);

private:
	uint8_t * mem;
	uint8_t * disk;
	uint16_t baseRegister;
	uint16_t limitRegister;

	ProcessInfo *processInfo;
	int currentPageIndex; // store last page index for FIFO placement 
};



#endif