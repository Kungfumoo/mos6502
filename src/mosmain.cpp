#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <fstream>
#include "MOS6502/MOS6502CPU.h"
#include "MOS6502/BasicMemory.h"

using namespace std;
using namespace MOS_6502;

vector<byte> fetchProgram(string filename)
{
    ifstream ifile(filename);

    if(!ifile.is_open())
        throw new logic_error("Could not open " + filename);

    vector<byte> program;

    char buffer;

    while(ifile.get(buffer))
        program.push_back((byte)buffer);

    ifile.close();

    return program;
}

void onCycle()
{
    cout << "cpu cycle" << endl;
}

int main(int argc, char* argv[])
{
    BasicMemory* memory = new BasicMemory();
    MOS6502CPU* cpu = new MOS6502CPU(1.19, memory, true);

    if(argc == 1)
    {
        cout << "No program to run provided! Please provide a compiled bin file to run" << endl;
        return 1;
    }

    string filename(argv[1]);

    //vector<byte> program = {0xA9, 0x2F, 0xC9, 0x2F, 0xD0, 0x03, 0x8D, 0x22, 0x00, 0x00};
    vector<byte> program = fetchProgram(filename);

    //TEMP: display contents of program in hex
    /*for(unsigned int i = 0; i < program.size(); i++)
        cout << "0x" << std::hex << (int)program[i] << endl;*/

    cpu->addCycleCallback(onCycle);

    try
    {
        cpu->execute(program);
    }
    catch (exception* e)
    {
        cout << "EXCEPTION: " << e->what() << endl;
    }

    cpu->status(false);

#ifdef _WIN32
	system("pause");
#endif

    delete cpu;
    return 0;
}
