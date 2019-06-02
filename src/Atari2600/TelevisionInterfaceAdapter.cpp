#include "TelevisionInterfaceAdapter.h"
#include "Memory.h"
#include <iostream>

using namespace Atari2600;
using namespace std;

typedef TelevisionInterfaceAdapter TIA;

//--Public:
const float TIA::CLOCK_SPEED = 3.58; //mhz

void TIA::runCycle()
{
    cout << "TIA: cycle!" << endl;
}

TIA::TelevisionInterfaceAdapter(Memory* memory)
{
    _memory = memory;
}