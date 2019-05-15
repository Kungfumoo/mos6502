//Memory class implementation
#include "BasicMemory.h"

using namespace MOS_6502;

//--General Methods
byte BasicMemory::read(unsigned short address)
{
	return _memory[address];
}

void BasicMemory::write(byte value, unsigned short address)
{
	_memory[address] = value;
}

//--Constructors
BasicMemory::BasicMemory() : Memory(MAX_SIZE_BYTES) {}
BasicMemory::BasicMemory(unsigned int bytes) : Memory(bytes) {}