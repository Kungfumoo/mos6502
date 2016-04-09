#include <iostream>
#include <exception>
#include "MOS6502CPU.h"
#include "BasicMemory.h"
#include "CpuTest.h"
#include "Compiler.h"

using namespace std;
using namespace MOS_6502;

int main()
{
    Memory* memory = new BasicMemory();
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

    //Compiler:
	Compiler* compiler = new Compiler();

#ifdef _WIN32
	string file = "C:\\Users\\Aaron Winter\\Documents\\Projects\\mos6502\\asmsrc\\test5.asm";
#else
    string file = "/home/aaron/Documents/Projects/mos6502/asmsrc/test5.asm";
#endif

	try
	{
		cout << "\nCompiler:" << endl;
		vector<byte> program = compiler->compileFromFile(file);

		cout << "PROGRAM:" << endl;
		for (auto i = program.begin(); i < program.end(); i++)
			cout << "0x" << hex << (int)*i << ", ";
		cout << endl;

		cout << "\nCPU:" << endl;
		cpu->execute(program);
		cpu->status(false);
	}
	catch (exception* e)
	{
		cout << "\nEXCEPTION: " << e->what() << endl;

#ifdef _WIN32
		system("pause");
#endif

		return 1;
	}

    delete cpu;
    delete testSuite;
	delete compiler;

#ifdef _WIN32
	system("pause");
#endif

    return 0;
}
