#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "Atari2600/DisplayAdapter/SFMLAdapter.h"
#include "Atari2600/Memory.h"
#include "Atari2600/TelevisionInterfaceAdapter.h"
#include "Atari2600/MOS6507.h"

using namespace Atari2600;
using namespace Atari2600::DisplayAdapter;
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
    MOS6507* cpu = new MOS6507(TelevisionInterfaceAdapter::CLOCK_SPEED / 3, memory, true);
    SFMLAdapter* displayAdapter = new SFMLAdapter(800, 600);
    TelevisionInterfaceAdapter* tia = new TelevisionInterfaceAdapter(displayAdapter, memory);

    float cpuClockSpeed = cpu->getClockSpeedMhz();
    int tiaCycles = TelevisionInterfaceAdapter::CLOCK_SPEED / cpuClockSpeed;

    cpu->addCycleCallback([&] () {
        //advance TIA
        for(int i = 0; i < tiaCycles; i++)
            if(tia->runCycle() == TIAState::WSYNC)
                break;
    });

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

    delete tia;
    delete cpu;

    return 0;
}