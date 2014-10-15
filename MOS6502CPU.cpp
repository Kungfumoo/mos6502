//MOS6502CPU class implementation
#include "MOS6502CPU.h"
#include "StatusRegister.h"
#include "Memory.h"
#include "Compiler.h"
#include "Exceptions.h"
#include <iostream>

using namespace MOS_6502;
using namespace std;

//Constant Definitions
byte MOS6502CPU::MAX_CLOCK_SPEED_MHZ = 2;

//--ADRESSING MODE METHODS(private):
unsigned short MOS6502CPU::getAbsolute()
{
    //[OC][p2][p1]
    //OC = opcode, p1 = part1 of address, p2 = part 2 of address
    byte opp2 = _memory->read(_programCounter++);
    byte opp1 = _memory->read(_programCounter++);
    unsigned short address = (opp1 << 8) | opp2;

    return address;
}

unsigned short MOS6502CPU::getPreIndirect()
{
    //get address of the start byte of the address of the operand.
    unsigned short startAddress = _memory->read(_programCounter++) + _x;

    //wrap around addition(to ensure its always a zero-page address):
    if(startAddress > 255)
        startAddress = startAddress - 256; //256 because 255 = 255, 256 = 0, 257 = 1

    //form operand address by concatanating the value contained in start+1 and start
    unsigned short operAddress = (_memory->read(startAddress+1) << 8) | _memory->read(startAddress);

    return operAddress;
}

unsigned short MOS6502CPU::getPostIndirect()
{
    //form bare address from the value given in the arguement and the next address
    unsigned short startAddress = _memory->read(_programCounter++);
    unsigned short address = (_memory->read(startAddress+1) << 8) | _memory->read(startAddress);

    return address + _y;
}

unsigned short MOS6502CPU::getZeroPageIndexed()
{
    //Work out operand, add actual operand to value in X to form the address to the value we want
    unsigned short address =  _memory->read(_programCounter++) + _x; //the address of the value

    /*NOTE:
     *-Should this always result in a zero-page result?
     *-should this use wrap around addition?
     *
     *At the moment, i'm assuming it always results in a zero-page and not wrap around, i need to find out.
     */

    if(address >= 0xFF)
        address = 0xFF;

    return address;
}

unsigned short MOS6502CPU::getRelative(byte value)
{
    unsigned short newAddress = _programCounter;

    if(value > 127) //negative number 128 = -128(twos complement)
        newAddress -= value;
    else //positive
        newAddress += value;

    return newAddress;
}

//--ASSEMBLY PROCEDURES(private):
void MOS6502CPU::ADC(byte operand)
{
    //Locals
    bool carry = _status->getC();
    bool BCD = _status->getD();

    if(BCD)
    {
        //TODO
    }
    else //normal binary numbers
    {
        unsigned short result = operand + _accumulator + ((carry) ? 1 : 0);

        if(result > 127) //negative number or overflow
        {
            if(result > 255) //overflow + carry
            {
                result = operand - 1; //-1 because of carry
                _status->setC(true);
                _status->setV(true);
                _status->setZ(result == 0);
                _status->setS(result > 127);
            }
            else //it's negative
            {
                _status->setC(false);
                _status->setV(false);
                _status->setZ(false);
                _status->setS(true);
            }
        }
        else //positive number
        {
            _status->setC(false);
            _status->setV(false);
            _status->setZ(result == 0);
            _status->setS(false);
        }

        _accumulator = (byte)result; //load result into accumulator
    }
}

void MOS6502CPU::ADC1()
{
    ADC(_memory->read(_programCounter++));
}

void MOS6502CPU::ADC3()
{
    ADC(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::ADC7() //indexed zero-page, so cannot lookup anything higher then $FF
{
    unsigned short address = getZeroPageIndexed();
    ADC(_memory->read(address));
}

void MOS6502CPU::ADC2()
{
    unsigned short address = getAbsolute();
    ADC(_memory->read(address));
}

void MOS6502CPU::ADC6_X()
{
    unsigned short address = getAbsolute();
    ADC(_memory->read(address + _x));
}

void MOS6502CPU::ADC6_Y()
{
    unsigned short address = getAbsolute();
    ADC(_memory->read(address + _y));
}

void MOS6502CPU::ADC9()
{
    unsigned short address = getPreIndirect();
    ADC(_memory->read(address));
}

void MOS6502CPU::ADC10()
{
    unsigned short address = getPostIndirect();
    ADC(_memory->read(address));
}

void MOS6502CPU::AND(byte operand)
{
    _accumulator &= operand;

    //set status
    _status->setZ(_accumulator == 0);
    _status->setS(_accumulator > 127);
}

void MOS6502CPU::AND1()
{
    AND(_memory->read(_programCounter++));
}

void MOS6502CPU::AND3()
{
    AND(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::AND7() //indexed zero-page, so cannot lookup anything higher then $FF
{
    unsigned short address = getZeroPageIndexed();
    AND(_memory->read(address));
}

void MOS6502CPU::AND2()
{
    unsigned short address = getAbsolute();
    AND(_memory->read(address));
}

void MOS6502CPU::AND6_X()
{
    unsigned short address = getAbsolute();
    AND(_memory->read(address + _x));
}

void MOS6502CPU::AND6_Y()
{
    unsigned short address = getAbsolute();
    AND(_memory->read(address + _y));
}

void MOS6502CPU::AND9()
{
    unsigned short address = getPreIndirect();
    AND(_memory->read(address));
}

void MOS6502CPU::AND10()
{
    unsigned short address = getPostIndirect();
    AND(_memory->read(address));
}

void MOS6502CPU::ASL(byte& operand)
{
    //locals
    byte value = operand << 1; //left shift by 1

    //set status
    _status->setC(operand > 127); //if the original operand is higher then 127, then bit 7 is 1, so it will be a carry after this operation
    _status->setZ(value == 0);
    _status->setS(value > 127); //not to be confused with setC, its using 'value' intentionally

    operand = value;
}

void MOS6502CPU::ASL5()
{
    ASL(_accumulator);
}

void MOS6502CPU::ASL3()
{
    byte address = _memory->read(_programCounter++); //byte because zeropage
    byte operand = _memory->read(address);
    ASL(operand);
    _memory->write(operand, address);
}

void MOS6502CPU::ASL7()
{
    unsigned short address = getZeroPageIndexed();
    byte operand = _memory->read(address);
    ASL(operand);
    _memory->write(operand, address);
}

void MOS6502CPU::ASL2()
{
    unsigned short address = getAbsolute();
    byte operand = _memory->read(address);
    ASL(operand);
    _memory->write(operand, address);
}

void MOS6502CPU::ASL6()
{
    unsigned short address = getAbsolute();
    byte operand = _memory->read(address + _x);
    ASL(operand);
    _memory->write(operand, address);
}

void MOS6502CPU::BCC11()
{
    //If carry clear, then branch to x destination
    if(!_status->getC())
        _programCounter = getRelative(_memory->read(_programCounter++));
}

void MOS6502CPU::BCS11()
{
    //if carry set, then branch to x destination
    if(_status->getC())
        _programCounter = getRelative(_memory->read(_programCounter++));
}

void MOS6502CPU::LDA1()
{
    //Locals
    byte operand = _memory->read(_programCounter++);

    //set status
    _status->setS(operand > 127);
    _status->setZ(operand == 0);

    _accumulator = operand;
}

//--General(private)
void MOS6502CPU::setupCycleLookup()
{
    for(int i = 0; i < 255; i++)
        _cycleLookup[i] = 0;

    //cycles for operations(TODO: FILL with actual values)
    _cycleLookup[0x00] = 7;
    _cycleLookup[0x01] = 6;
    _cycleLookup[0x05] = 3;
    _cycleLookup[0x06] = 5;
    _cycleLookup[0x08] = 3;
    _cycleLookup[0x09] = 2;
    _cycleLookup[0x0A] = 2;
    _cycleLookup[0x0D] = 4;
    _cycleLookup[0x0E] = 7;
    _cycleLookup[0x10] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0x11] = 5; //(+1 if page crossed)
    _cycleLookup[0x15] = 4;
    _cycleLookup[0x16] = 6;
    _cycleLookup[0x18] = 2;
    _cycleLookup[0x19] = 4; //(+1 if page crossed)
    _cycleLookup[0x1D] = 4; //(+1 if page crossed)
    _cycleLookup[0x1E] = 7;
    _cycleLookup[0x20] = 6;
    _cycleLookup[0x21] = 6;
    _cycleLookup[0x24] = 3;
    _cycleLookup[0x25] = 3;
    _cycleLookup[0x26] = 5;
    _cycleLookup[0x28] = 4;
    _cycleLookup[0x29] = 2;
    _cycleLookup[0x2A] = 2;
    _cycleLookup[0x2C] = 4;
    _cycleLookup[0x2D] = 4;
    _cycleLookup[0x2E] = 6;
    _cycleLookup[0x30] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0x31] = 5; //(+1 if page crossed)
    _cycleLookup[0x35] = 4;
    _cycleLookup[0x36] = 6;
    _cycleLookup[0x38] = 2;
    _cycleLookup[0x39] = 4; //(+1 if page crossed)
    _cycleLookup[0x3D] = 4; //(+1 if page crossed)
    _cycleLookup[0x3E] = 7;
    _cycleLookup[0x40] = 6;
    _cycleLookup[0x41] = 6;
    _cycleLookup[0x45] = 3;
    _cycleLookup[0x46] = 5;
    _cycleLookup[0x48] = 3;
    _cycleLookup[0x49] = 2;
    _cycleLookup[0x4A] = 2;
    _cycleLookup[0x4C] = 3;
    _cycleLookup[0x4D] = 4;
    _cycleLookup[0x4E] = 6;
    _cycleLookup[0x50] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0x51] = 5; //(+1 if page crossed)
    _cycleLookup[0x55] = 4;
    _cycleLookup[0x56] = 6;
    _cycleLookup[0x58] = 2;
    _cycleLookup[0x59] = 4; //(+1 if page crossed)
    _cycleLookup[0x5D] = 4; //(+1 if page crossed)
    _cycleLookup[0x5E] = 7;
    _cycleLookup[0x60] = 6;
    _cycleLookup[0x61] = 6;
    _cycleLookup[0x65] = 3;
    _cycleLookup[0x66] = 5;
    _cycleLookup[0x68] = 4;
    _cycleLookup[0x69] = 2;
    _cycleLookup[0x6A] = 2;
    _cycleLookup[0x6C] = 5;
    _cycleLookup[0x6D] = 4;
    _cycleLookup[0x6E] = 6;
    _cycleLookup[0x70] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0x71] = 5; //(+1 if page crossed)
    _cycleLookup[0x75] = 4;
    _cycleLookup[0x76] = 6;
    _cycleLookup[0x78] = 2;
    _cycleLookup[0x79] = 4; //(+1 if page crossed)
    _cycleLookup[0x7D] = 4; //(+1 if page crossed)
    _cycleLookup[0x7E] = 7;
    _cycleLookup[0x81] = 6;
    _cycleLookup[0x84] = 3;
    _cycleLookup[0x85] = 3;
    _cycleLookup[0x86] = 3;
    _cycleLookup[0x88] = 2;
    _cycleLookup[0x8A] = 2;
    _cycleLookup[0x8C] = 4;
    _cycleLookup[0x8D] = 4;
    _cycleLookup[0x8E] = 4;
    _cycleLookup[0x90] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0x91] = 6;
    _cycleLookup[0x94] = 4;
    _cycleLookup[0x95] = 4;
    _cycleLookup[0x96] = 4;
    _cycleLookup[0x98] = 2;
    _cycleLookup[0x99] = 5;
    _cycleLookup[0x9A] = 2;
    _cycleLookup[0x9D] = 5;
    _cycleLookup[0xA0] = 2;
    _cycleLookup[0xA1] = 6;
    _cycleLookup[0xA2] = 2;
    _cycleLookup[0xA4] = 3;
    _cycleLookup[0xA5] = 3;
    _cycleLookup[0xA6] = 3;
    _cycleLookup[0xA8] = 2;
    _cycleLookup[0xA9] = 2;
    _cycleLookup[0xAA] = 2;
    _cycleLookup[0xAC] = 4;
    _cycleLookup[0xAD] = 4;
    _cycleLookup[0xAE] = 4;
    _cycleLookup[0xB0] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0xB1] = 5;
    _cycleLookup[0xB4] = 4;
    _cycleLookup[0xB5] = 4;
    _cycleLookup[0xB6] = 4;
    _cycleLookup[0xB8] = 2;
    _cycleLookup[0xB9] = 4; //(+1 if page crossed)
    _cycleLookup[0xBA] = 2;
    _cycleLookup[0xBC] = 4; //(+1 if page crossed)
    _cycleLookup[0xBD] = 4; //(+1 if page crossed)
    _cycleLookup[0xBE] = 4; //(+1 if page crossed)
    _cycleLookup[0xC0] = 2;
    _cycleLookup[0xC1] = 6;
    _cycleLookup[0xC4] = 3;
    _cycleLookup[0xC5] = 3;
    _cycleLookup[0xC6] = 5;
    _cycleLookup[0xC8] = 2;
    _cycleLookup[0xC9] = 2;
    _cycleLookup[0xCA] = 2;
    _cycleLookup[0xCC] = 4;
    _cycleLookup[0xCD] = 4;
    _cycleLookup[0xCE] = 6;
    _cycleLookup[0xD0] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0xD1] = 5; //(+1 if page crossed)
    _cycleLookup[0xD5] = 4;
    _cycleLookup[0xD6] = 6;
    _cycleLookup[0xD8] = 2;
    _cycleLookup[0xD9] = 4; //(+1 if page crossed)
    _cycleLookup[0xDD] = 4; //(+1 if page crossed)
    _cycleLookup[0xDE] = 7;
    _cycleLookup[0xE0] = 2;
    _cycleLookup[0xE1] = 6;
    _cycleLookup[0xE4] = 3;
    _cycleLookup[0xE5] = 3;
    _cycleLookup[0xE6] = 5;
    _cycleLookup[0xE8] = 2;
    _cycleLookup[0xE9] = 2;
    _cycleLookup[0xEA] = 2;
    _cycleLookup[0xEC] = 4;
    _cycleLookup[0xED] = 4;
    _cycleLookup[0xEE] = 6;
    _cycleLookup[0xF0] = 2; //(+1 if branch succeeds +2 if to a new page)
    _cycleLookup[0xF1] = 5;
    _cycleLookup[0xF5] = 4;
    _cycleLookup[0xF6] = 6;
    _cycleLookup[0xF8] = 2;
    _cycleLookup[0xF9] = 4;
    _cycleLookup[0xFD] = 4; //(+1 if page crossed)
    _cycleLookup[0xFE] = 7;
}

void MOS6502CPU::run()
{
    int counter = _cycles;
    unsigned int addressingSpace = _memory->getSize();

    while(_running)
    {
        //fetch op code
        byte opcode = _memory->read(_programCounter++);

        //remove number of cycles from counter:
        counter -= _cycleLookup[opcode];

        //execute op code
        runCommand(opcode);

        if(counter <= 0)
        {
            //Perform interupts

            //Perform cyclic tasks

            counter += _cycles;
        }

        counter--;
    }
}

void MOS6502CPU::runCommand(byte opcode)
{
    switch(opcode)
    {
    case 0x06: ASL3(); break;
    case 0x0E: ASL2(); break;
    case 0x16: ASL7(); break;
    case 0x1E: ASL6(); break;
    case 0x21: AND9(); break;
    case 0x25: AND3(); break;
    case 0x29: AND1(); break;
    case 0x2D: AND2(); break;
    case 0x31: AND10(); break;
    case 0x35: AND7(); break;
    case 0x39: AND6_Y(); break;
    case 0x3D: AND6_X(); break;
    case 0x61: ADC9(); break;
    case 0x65: ADC3(); break;
    case 0x69: ADC1(); break;
    case 0x6D: ADC2(); break;
    case 0x71: ADC10(); break;
    case 0x75: ADC7(); break;
    case 0x79: ADC6_Y(); break;
    case 0x7D: ADC6_X(); break;
    case 0x0A: ASL5(); break;
    case 0xA9: LDA1(); break;

    default:
        throw new UnknownOpCodeException(opcode);
    }
}

//--General(public)
void MOS6502CPU::execute(string program)
{

}

void MOS6502CPU::reset()
{
    _memory->clear();
    _running = false;

    //reset registers
    _status->reset();
    _accumulator = 0;
    _x = 0;
    _y = 0;
    _programCounter = 0;
    _stackPointer = 0;
}

void MOS6502CPU::start()
{
    if(_debug)
        cout << "Started CPU....." << endl;

    _running = true;
    run();
}

void MOS6502CPU::stop()
{

}

//--Debugging Methods
void MOS6502CPU::setPC(unsigned short address)
{
    _programCounter = address;
}

void MOS6502CPU::runNext(bool status)
{
    byte opcode = _memory->read(_programCounter++);

    try
    {
        runCommand(opcode);
    }
    catch(exception* e)
    {
        throw e;
    }

    if(status)
        this->status();
}

void MOS6502CPU::status()
{
    cout << "\n======================STATUS======================" << endl;
    cout << "Registers:" << endl;
    cout << "PC: 0x" << hex << _programCounter << endl;
    cout << "AC: 0x" << hex << (int)_accumulator << endl; //int cast because its interpreting it as char, not a number
    cout << "XR: 0x" << hex << (int)_x << endl;
    cout << "YR: 0x" << hex << (int)_y << endl;
    cout << "SP: 0x" << hex << (int)_stackPointer << endl << endl;

    cout << "Status Register:" << endl;
    cout << "|S|V| |B|D|I|Z|C|" << endl;
    cout << "|" << (_status->getS()) ? 1 : 0;
    cout << "|" << (_status->getV()) ? 1 : 0;
    cout << "|1";
    cout << "|" << (_status->getB()) ? 1 : 0;
    cout << "|" << (_status->getD()) ? 1 : 0;
    cout << "|" << (_status->getI()) ? 1 : 0;
    cout << "|" << (_status->getZ()) ? 1 : 0;
    cout << "|" << ((_status->getC()) ? 1 : 0) << endl;

    cout << "\n=======================END========================\n" << endl;
}

void MOS6502CPU::status(string header)
{
    cout << "\n======================" << header << "======================" << endl;
    cout << "Registers:" << endl;
    cout << "PC: 0x" << hex << _programCounter << endl;
    cout << "AC: 0x" << hex << (int)_accumulator << endl; //int cast because its interpreting it as char, not a number
    cout << "XR: 0x" << hex << (int)_x << endl;
    cout << "YR: 0x" << hex << (int)_y << endl;
    cout << "SP: 0x" << hex << (int)_stackPointer << endl << endl;

    cout << "Status Register:" << endl;
    cout << "|S|V| |B|D|I|Z|C|" << endl;
    cout << "|" << (_status->getS()) ? 1 : 0;
    cout << "|" << (_status->getV()) ? 1 : 0;
    cout << "|1";
    cout << "|" << (_status->getB()) ? 1 : 0;
    cout << "|" << (_status->getD()) ? 1 : 0;
    cout << "|" << (_status->getI()) ? 1 : 0;
    cout << "|" << (_status->getZ()) ? 1 : 0;
    cout << "|" << ((_status->getC()) ? 1 : 0) << endl;

    cout << "\n=======================END========================\n" << endl;
}

//--Constructors
//Constructor
MOS6502CPU::MOS6502CPU(unsigned int clockSpeedMhz, Memory* memory, bool debug)
{
    //Other
    _clockSpeed = (clockSpeedMhz > MAX_CLOCK_SPEED_MHZ) ? MAX_CLOCK_SPEED_MHZ: clockSpeedMhz;
    _cycles = _clockSpeed * 1000000;
    _cycleLookup = new byte[255]; //0xFF
    _memory = memory;
    _compiler = new Compiler(this);
    _running = false;
    _debug = debug;

    //Setup registers
    _status = new StatusRegister();
    _accumulator = 0;
    _x = 0;
    _y = 0;
    _programCounter = 0;
    _stackPointer = 0;
}

//Destructor
MOS6502CPU::~MOS6502CPU()
{
    delete _status;
    delete _memory;
}
