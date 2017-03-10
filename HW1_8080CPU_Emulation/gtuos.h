#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"
#include <string>


class GTUOS{

	public:
		GTUOS();
		uint64_t handleCall(const CPU8080& cpu8080);
        void saveMemoryContents(std::ofstream& output,const CPU8080& cpu);

    int readB(const CPU8080& cpu8080);

    private:
        int printB(const CPU8080& cpu8080);
        int printStr(const CPU8080& cpu8080);
        int printMem(const CPU8080& cpu8080);


        int readMem(const CPU8080& cpu8080);
        int readStr(const CPU8080& cpu8080);

        enum CALL_TYPE{
            PRINT_B=1,
            PRINT_MEM=2,
            READ_B=3,
            READ_MEM=4,
            PRINT_STR=5,
            READ_STR=6
        };

        const int PRINT_B_CYCLE=10;
        const int PRINT_MEM_CYCLE=10;
        const int READ_B_CYCLE=10;
        const int READ_MEM_CYCLE=10;
        const int PRINT_STR_CYCLE=100;
        const int READ_STR_CYCLE=100;


};

#endif
