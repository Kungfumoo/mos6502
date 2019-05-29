#include "Memory.h"

using namespace Atari2600;

//--Constant definition
const unsigned short Memory::MAX_SIZE_BYTES = 8192;
const unsigned short ROM_START_ADDR = 0x1000;

//--Methods
void Memory::loadRom(std::vector<byte> program)
{
    //TODO: clear down ROM and load program
}

//--Constructors
Memory::Memory() : MOS_6502::BasicMemory(MAX_SIZE_BYTES) {}