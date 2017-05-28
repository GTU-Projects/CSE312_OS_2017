#ifndef MMU_HM_HEADER
#define MMU_HM_HEADER

#include <cstdlib>
#include "memoryBase.h"

#define SIZE1K 1024
#define SIZE8K 8192

/*
	VA = Page(4bit -> 2^4 : 16 PageNum) + Offset(10bit -> 2^10 : 1KB PageSize)

	PA = Frame(3bit -> 2^3: 8 PageNum for mem) + Offset(10bit -> 2^10 : 1KB PageSize)

*/

typedef struct{
	uint8_t modifiedBit;  
	uint8_t referencedBit;
	uint8_t present;
	uint8_t frame; // 
}Page_t;


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

private:
	uint8_t * mem;
	uint8_t * disk;
	uint16_t baseRegister;
	uint16_t limitRegister;

};



#endif