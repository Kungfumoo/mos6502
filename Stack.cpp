//Stack class implementation
#include "Stack.h"
#include "MOS6502CPU.h"

using namespace MOS_6502;

//--Constant Definitions
byte Stack::TOP = 0xFF;

//--Public Methods
void Stack::push(byte data)
{
    //TODO: write item at SP, decrement SP
}

byte Stack::pop()
{
    //TODO: read and cache copy of item from SP, delete item @ SP, increment SP
}

unsigned int Stack::size()
{
    //TODO: calculate size by (TOP - StackPointer)
}

void Stack::reset()
{
    //TODO: delete contents of stack and reset pointer to TOP
}

//--Constructors
//Constructor
Stack::Stack(MOS6502CPU* cpu)
{
    _cpu = cpu;
}
