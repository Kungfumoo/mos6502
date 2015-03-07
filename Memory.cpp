//Memory class implementation
#include "Memory.h"

using namespace MOS_6502;

//--Constant definition
const unsigned int Memory::MAX_SIZE_BYTES = 65536;

//--Accessors
unsigned int Memory::getSize()
{
    return _size;
}

//--General Methods
byte Memory::read(unsigned short address)
{
    /*
     * TODO: May need to add more here to depending on how the atari handles memory access
     * eg: memory mirroring etc..
     */
    return _memory[address];
}

void Memory::write(byte value, unsigned short address)
{
    /*
     * TODO: May need to add more here to depending on how the atari handles memory access
     * eg: memory mirroring etc..
     */
    _memory[address] = value;
}

void Memory::clear()
{
    for(unsigned int i = 0; i < _size; i++)
        _memory[i] = 0;
}

//--Constructors
//Constructor
Memory::Memory(unsigned int bytes)
{
    if(bytes > MAX_SIZE_BYTES)
        bytes = MAX_SIZE_BYTES;

    _memory = new byte[bytes];
    _size = bytes;
    clear();
}

//Destructor
Memory::~Memory()
{
    delete[] _memory;
}
