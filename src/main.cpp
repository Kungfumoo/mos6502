#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "Atari2600/Memory.h"
#include "MOS6502/MOS6502CPU.h"

using namespace Atari2600;
using namespace MOS_6502;
using namespace std;

typedef Atari2600::Memory ATMemory;

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

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        cout << "No program to run provided! Please provide a compiled bin file to run" << endl;
        return 1;
    }

    string filename(argv[1]);
    vector<byte> program = fetchProgram(filename);

    ATMemory* memory = new ATMemory();
    MOS6502CPU* cpu = new MOS6502CPU(1.19, memory, true);

    try
    {
        memory->loadRom(program);
        cpu->start();
        cpu->status(false);
    }
    catch (exception* e)
    {
        cout << "EXCEPTION: " << e->what() << endl;
    }

    delete cpu;

    return 0;
}