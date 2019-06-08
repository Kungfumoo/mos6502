#include "TelevisionInterfaceAdapter.h"
#include "Memory.h"
#include <iostream>

using namespace Atari2600;
using namespace std;

typedef TelevisionInterfaceAdapter TIA;

//--Private:
const byte TIA::CLOCKS_PER_SCANLINE = 228;
const unsigned short TIA::MAX_SCANLINES = 262;
const byte TIA::VERTICAL_PICTURE_THRESHOLD = 40;
const byte TIA::HORIZONTAL_PICTURE_THRESHOLD = 68;

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

        //Ready to draw visible lines
        if(_vScanlineCounter > VERTICAL_PICTURE_THRESHOLD &&
           _clockCounter > HORIZONTAL_PICTURE_THRESHOLD)
        {
            cout << "TIA: render!" << endl;
        }
        
        if(_clockCounter-- == 0)
        {
            _clockCounter = CLOCKS_PER_SCANLINE;

            //TODO: not sure what is involved when a full frame has been rendered
            if(_vScanlineCounter++ > MAX_SCANLINES)
                _vScanlineCounter = 0;
        }

        return false;
    }
}

TIA::TelevisionInterfaceAdapter(Memory* memory)
{
    _clockCounter = CLOCKS_PER_SCANLINE;
    _vScanlineCounter = 0;
    _memory = memory;
}