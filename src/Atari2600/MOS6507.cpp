#include "MOS6507.h"
#include "../MOS6502/Memory.h"

using namespace Atari2600;
using namespace MOS_6502;

const unsigned short MOS6507::ADDRESS_LIMIT = 0x1FFF;

//--ADRESSING MODE METHODS(protected):
unsigned short MOS6507::getAbsolute()
{
    unsigned short address = MOS6502CPU::getAbsolute();

    if(address >= ADDRESS_LIMIT) //clear last 3 bits
        for(int n = 15; n >= 13; n--)
            address &= ~(1UL << n);

    return address;
}

//--Constructors
MOS6507::MOS6507(float clockSpeedMhz, Memory* memory, bool debug)
    :MOS6502CPU(clockSpeedMhz, (MOS_6502::Memory*)memory, debug)
{}

//--Destructors
MOS6507::~MOS6507()
{}