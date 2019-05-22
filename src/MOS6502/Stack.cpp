//Stack class implementation
#include "Stack.h"
#include "MOS6502CPU.h"
#include "Memory.h"

using namespace MOS_6502;

//--Constant Definitions
byte Stack::TOP = 0xFF;
unsigned short Stack::HIGH = 0x0100;

//--Public Methods
void Stack::push(byte data)
{
    //Locals
    Memory* memory = _cpu->_memory;

    memory->write(data, (_cpu->_stackPointer)-- + HIGH);
    _size++;
}

byte Stack::pop()
{
    //Locals
    Memory* memory = _cpu->_memory;
    byte data = memory->read(++(_cpu->_stackPointer) + HIGH);

    //delete
    memory->write(0x0, _cpu->_stackPointer + HIGH);
    _size--;

    return data;
}

unsigned int Stack::size()
{
    return _size;
}

void Stack::reset()
{
    //Locals
    Memory* memory = _cpu->_memory;

    for(unsigned short i = HIGH; i < HIGH + TOP; i++)
        memory->write(0x0, i);

    _size = 0;
    _cpu->_stackPointer = TOP;
}

//--Operators
byte Stack::operator[](byte i)
{
    //Locals
    Memory* memory = _cpu->_memory;

    if(i < _size)
        return memory->read(HIGH + ((_cpu->_stackPointer + 1) + i));
    else
    {
        //TODO: throw some sort of access exception.
        return 0; //temp
    }
}

//--Constructors
//Constructor
Stack::Stack(MOS6502CPU* cpu)
{
    _cpu = cpu;
    _size = 0;
}
