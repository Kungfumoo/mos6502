//StatusRegister class implementation
#include "StatusRegister.h"
#include "Utility.h"

using namespace std;
using namespace MOS_6502;

//--Accessor
bool StatusRegister::getC()
{
    return _register[0];
}

bool StatusRegister::getZ()
{
    return _register[1];
}

bool StatusRegister::getI()
{
    return _register[2];
}

bool StatusRegister::getD()
{
    return _register[3];
}

bool StatusRegister::getB()
{
    return _register[4];
}

bool StatusRegister::getV()
{
    return _register[6];
}

bool StatusRegister::getS()
{
    return _register[7];
}

//--Mutators
void StatusRegister::setC(bool value)
{
    _register[0] = value;
}

void StatusRegister::setZ(bool value)
{
    _register[1] = value;
}

void StatusRegister::setI(bool value)
{
    _register[2] = value;
}

void StatusRegister::setD(bool value)
{
    _register[3] = value;
}

void StatusRegister::setB(bool value)
{
    _register[4] = value;
}

void StatusRegister::setV(bool value)
{
    _register[6] = value;
}

void StatusRegister::setS(bool value)
{
    _register[7] = value;
}

//--General Methods
void StatusRegister::reset()
{
    //set all to 0 apart from 5
    for(byte i = 0; i < 8; i++)
        _register[i] = false;

    _register[5] = true;
}

string StatusRegister::toString()
{
    //locals
    string binaryString;

    for(int i = 0; i < 8; i++)
        binaryString += (_register[i] == true) ? '1' : '0';

    return binaryString;
}

byte StatusRegister::toByte()
{
    return Utility::toByte(_register);
}

void StatusRegister::fromByte(byte value)
{
    _register = Utility::toBinary(value);
}

//--Constructors
//Constructor
StatusRegister::StatusRegister()
{
    //set all to 0 apart from 5
    for(byte i = 0; i < 8; i++)
        _register[i] = false;

    _register[5] = true;
}

//Destructor
StatusRegister::~StatusRegister() {}
