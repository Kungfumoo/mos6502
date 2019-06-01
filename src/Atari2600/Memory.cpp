#include "Memory.h"
#include <stdexcept>

using namespace std;
using namespace Atari2600;

//--Constant definition
const unsigned short Memory::MAX_SIZE_BYTES = 65535;
const unsigned short Memory::ROM_START_ADDR = 0x1000;
const unsigned short Memory::ROM_END_ADDR = Memory::MAX_SIZE_BYTES;

//--Methods
void Memory::loadRom(std::vector<byte>& program)
{
    if(program.size() > (ROM_END_ADDR - ROM_START_ADDR))
        throw new length_error("Program size exceeds addressable memory!");

    int start = ROM_START_ADDR;

    //Load program into ROM
    for(auto i = program.begin(); i < program.end(); i++)
        _memory[start++] = *i;

    //fill rest of ROM with 0
    while(start <= ROM_END_ADDR)
        _memory[start++] = 0;
}

//--Constructors
Memory::Memory() : MOS_6502::BasicMemory(MAX_SIZE_BYTES) {}