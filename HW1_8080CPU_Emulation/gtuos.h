#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"


class GTUOS{

	public:
		GTUOS();
		uint64_t handleCall(const CPU8080& cpu8080,int mode);

    private:
    void printB(const CPU8080& cpu8080, int mode);
    void printStr(const CPU8080& cpu8080, int mode);

        enum CALL_TYPE{
            PRINT_B=1,
            PRINT_MEM=2,
            READ_B=3,
            READ_MEM=4,
            PRINT_STR=5,
            READ_STR=6
        };


};

#endif
