#include "TelevisionInterfaceAdapter.h"
#include "Memory.h"
#include <iostream>

using namespace Atari2600;
using namespace std;

typedef TelevisionInterfaceAdapter TIA;

//--Private:
const byte TIA::CLOCKS_PER_SCANLINE = 228;

//--Public:
const float TIA::CLOCK_SPEED = 3.58; //mhz

bool TIA::runCycle()
{
    if(_memory->isWSYNC()) //handle WSYNC
    {
        cout << "TIA: WSYNC" << endl;

        byte remainingCycles = CLOCKS_PER_SCANLINE - _clockCounter;
        while(remainingCycles-- > 0)
            runCycle();

        return true;
    }
    else
    {
        cout << "TIA: cycle!" << endl;
        
        if(_clockCounter-- == 0)
            _clockCounter = CLOCKS_PER_SCANLINE;

        return false;
    }
}

TIA::TelevisionInterfaceAdapter(Memory* memory)
{
    _clockCounter = CLOCKS_PER_SCANLINE;
    _memory = memory;
}