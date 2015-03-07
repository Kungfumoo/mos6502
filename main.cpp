#include <iostream>
#include <exception>
#include "MOS6502CPU.h"
#include "Memory.h"
#include "CpuTest.h"

using namespace std;
using namespace MOS_6502;

int main()
{
    Memory* memory = new Memory(64000); //64kb
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);

    /*My Playground:
    cpu->status("INITIAL");

    unsigned short startAddress = 0x600;
    cpu->setPC(startAddress); //set program counter to point to inital address

    //write the program to memory
    memory->write(0xA9, startAddress++); //write LDA instruction to first byte
    memory->write(0xFF, startAddress++); //LDA operand
    memory->write(0x69, startAddress++); //write ADC instruction to next byte
    memory->write(0x0A, startAddress++); //ADC operand

    //run program:
    try
    {
        cpu->runNext(true);
        cpu->runNext(true);
    }
    catch(exception* e)
    {
        cout << "ERROR: " << e->what() << endl;
        return 1;
    }*/

    //TESTS:
    CpuTest* testSuite = new CpuTest();
    testSuite->runTests();

    delete cpu;
    delete testSuite;

#ifdef _WIN32
	system("pause");
#endif

    return 0;
}
