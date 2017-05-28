#include <iostream>
#include "MMU_HM.h"
#include "general_types.h"


MMU_HM::MMU_HM(uint64_t memSize, uint64_t diskSize) {
	mem = (uint8_t*) calloc(memSize, sizeof(uint8_t));
	disk = (uint8_t*) calloc(diskSize, sizeof(uint8_t));
	baseRegister = 0;
	limitRegister = 0;
	currentPageIndex=0;
}

MMU_HM::~MMU_HM() {
	free(disk);
	free(mem);
}

void MMU_HM::setProcessInfo(ProcessInfo *processInfo) {
	this->processInfo = processInfo;
}

uint8_t & MMU_HM::at(uint32_t ind) {

	int realNumber = ind + baseRegister;
	

	int pageNumber = realNumber / SIZE1K;
	int offset = realNumber % SIZE1K;
	// page table icinde var ise direk olarak memoryden okuyabilirim
	int frame;
	if (processInfo->pageTable[pageNumber].present) {
		//printf("Page:%d\n",pageNumber);
		frame = processInfo->pageTable[pageNumber].frame;
	} else {
		printf(RED "PageFault " RESET);

		++currentPageIndex; // next page location
		if (currentPageIndex > PAGE_TABLE_SIZE)
			currentPageIndex == 0;

		// load page to next location
		loadPageAt(currentPageIndex*SIZE1K, pageNumber*SIZE1K);

		processInfo->pageTable[pageNumber].present=1;
		processInfo->pageTable[pageNumber].frame=currentPageIndex;
		frame = currentPageIndex;
	}

	return mem[frame * SIZE1K + offset];

}

uint8_t & MMU_HM::physicalAt(uint32_t ind) {
	return disk[ind];
}

// Initializes first process loading to memory and page table setting
void MMU_HM::initMM() {

	for (int i = 0; i < SIZE1K; ++i) {
		mem[i] = disk[i];
	}
	limitRegister = SIZE8K;

	processInfo->pageTable[0].frame = 0;
	processInfo->pageTable[0].present = 1;
	processInfo->pageTable[0].referencedBit = 1;
}

// copy disk to file
void MMU_HM::loadPageAt(int memPageIndex, int diskPageIndex){
	printf(GRN "Load page from:%d to:%d\n" RESET,diskPageIndex,memPageIndex);

	for(int i=0;i<SIZE1K;++i){
		mem[memPageIndex+i] = disk[diskPageIndex+i];
		/*if(i<20)
		printf("%x - %x\n",mem[memPageIndex+i] , disk[diskPageIndex+i]);*/
	}

}



