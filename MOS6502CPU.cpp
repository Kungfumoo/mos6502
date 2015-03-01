//MOS6502CPU class implementation
#include "MOS6502CPU.h"
#include "StatusRegister.h"
#include "Memory.h"
#include "Compiler.h"
#include "Stack.h"
#include "Exceptions.h"
#include "Utility.h"
#include <iostream>

using namespace MOS_6502;
using namespace std;

//Constant Definitions
byte MOS6502CPU::MAX_CLOCK_SPEED_MHZ = 2;
byte MOS6502CPU::NEGATIVE = 127;

//--STATE METHODS(private)
void MOS6502CPU::saveProgramCounter()
{
    byte pc[] = {(byte)((_programCounter >> 8) & 255), (byte)(_programCounter & 255)}; //split PC into two parts

    _stack->push(pc[0]);
    _stack->push(pc[1]);
}

void MOS6502CPU::getProgramCounter()
{
    byte pc[2];

    pc[1] = _stack->pop();
    pc[0] = _stack->pop();

    _programCounter = (pc[0] << 8) | pc[1];
}

void MOS6502CPU::saveCurrentState()
{
    byte status = _status->toByte();

    saveProgramCounter();
    _stack->push(status);
}

void MOS6502CPU::getLastState()
{
    byte status = _stack->pop();

    _status->fromByte(status);
    getProgramCounter();
}

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

unsigned short MOS6502CPU::getZeroPageIndexed(byte regValue)
{
    //Work out operand, add actual operand to value in X/Y to form the address to the value we want
    unsigned short address =  _memory->read(_programCounter++) + regValue; //the address of the value

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

    if(value > NEGATIVE) //negative number 128 = 0, 129 = -1, 130 = -2 ...
        newAddress += (128 - value);
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

        if(result > NEGATIVE) //negative number or overflow
        {
            if(result > 255) //overflow + carry
            {
                result = operand - 1; //-1 because of carry
                _status->setC(true);
                _status->setV(true);
                _status->setZ(result == 0);
                _status->setS(result > NEGATIVE);
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
    unsigned short address = getZeroPageIndexed(_x);
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
    _status->setS(_accumulator > NEGATIVE);
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
    unsigned short address = getZeroPageIndexed(_x);
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
    _status->setC(operand > NEGATIVE); //if the original operand is higher then 127, then bit 7 is 1, so it will be a carry after this operation
    _status->setZ(value == 0);
    _status->setS(value > NEGATIVE); //not to be confused with setC, its using 'value' intentionally

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
    unsigned short address = getZeroPageIndexed(_x);
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
    byte arg = _memory->read(_programCounter++);

    //If carry clear, then branch to x destination
    if(!_status->getC())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BCS11()
{
    byte arg = _memory->read(_programCounter++);

    //if carry set, then branch to x destination
    if(_status->getC())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BEQ11()
{
    byte arg = _memory->read(_programCounter++);

    //if zero set, then branch
    if(_status->getZ())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BIT(byte operand)
{
    bitset<8> binOperand = Utility::toBinary(operand);

    _status->setV(binOperand[6]);
    _status->setS(binOperand[7]);
    _status->setZ((operand & _accumulator) == 0);
}

void MOS6502CPU::BIT3()
{
    BIT(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::BIT2()
{
    unsigned short address = getAbsolute();
    BIT(_memory->read(address));
}

void MOS6502CPU::BMI11()
{
    byte arg = _memory->read(_programCounter++);

    //if negative, then branch
    if(_status->getS())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BNE11()
{
    byte arg = _memory->read(_programCounter++);

    //if not zero, then branch
    if(!_status->getZ())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BPL11()
{
    byte arg = _memory->read(_programCounter++);

    //if not negative, then branch
    if(!_status->getS())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BVC11()
{
    byte arg = _memory->read(_programCounter++);

    //if no overflow then branch
    if(!_status->getV())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BVS11()
{
    byte arg = _memory->read(_programCounter++);

    //if overflow then branch
    if(_status->getV())
        _programCounter = getRelative(arg);
}

void MOS6502CPU::BRK4()
{
    _programCounter++;

    //Set break and interupt and save the state
    _status->setB(true);
    saveCurrentState();
    _status->setI(true);

    //Move program counter
    _programCounter = (_memory->read(0xFFFF) << 8) | _memory->read(0xFFFE);
}

void MOS6502CPU::CLC4()
{
    _status->setC(false);
}

void MOS6502CPU::CLD4()
{
    _status->setD(false);
}

void MOS6502CPU::CLI4()
{
    _status->setI(false);
}

void MOS6502CPU::CLV4()
{
    _status->setV(false);
}

void MOS6502CPU::CMP(byte operand)
{
    /* Sets status C, Z and S depending on results
     * C = if A >= O
     * Z = if A = 0
     * S = above is false
     */

    _status->setS(false);
    _status->setC(false);
    _status->setZ(false);

    if(_accumulator > operand)
        _status->setC(true);
    else if(_accumulator == operand)
    {
        _status->setC(true);
        _status->setZ(true);
    }
    else
        _status->setS(true);
}

void MOS6502CPU::CMP1()
{
    CMP(_memory->read(_programCounter++));
}

void MOS6502CPU::CMP3()
{
    CMP(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::CMP7()
{
    unsigned short address = getZeroPageIndexed(_x);
    CMP(_memory->read(address));
}

void MOS6502CPU::CMP2()
{
    unsigned short address = getAbsolute();
    CMP(_memory->read(address));
}

void MOS6502CPU::CMP6_X()
{
    unsigned short address = getAbsolute();
    CMP(_memory->read(address + _x));
}

void MOS6502CPU::CMP6_Y()
{
    unsigned short address = getAbsolute();
    CMP(_memory->read(address + _y));
}

void MOS6502CPU::CMP9()
{
    unsigned short address = getPreIndirect();
    CMP(_memory->read(address));
}

void MOS6502CPU::CMP10()
{
    unsigned short address = getPostIndirect();
    CMP(_memory->read(address));
}

void MOS6502CPU::CPX(byte operand)
{
    /* Sets status C, Z and S depending on results
     * C = if X >= O
     * Z = if X = 0
     * S = above is false
     */

    _status->setS(false);
    _status->setC(false);
    _status->setZ(false);

    if(_x > operand)
        _status->setC(true);
    else if(_x == operand)
    {
        _status->setC(true);
        _status->setZ(true);
    }
    else
        _status->setS(true);
}

void MOS6502CPU::CPX1()
{
    CPX(_memory->read(_programCounter++));
}

void MOS6502CPU::CPX3()
{
    CPX(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::CPX2()
{
    unsigned short address = getAbsolute();
    CPX(_memory->read(address));
}

void MOS6502CPU::CPY(byte operand)
{
    /* Sets status C, Z and S depending on results
     * C = if Y >= O
     * Z = if Y = 0
     * S = above is false
     */

    _status->setS(false);
    _status->setC(false);
    _status->setZ(false);

    if(_y > operand)
        _status->setC(true);
    else if(_y == operand)
    {
        _status->setC(true);
        _status->setZ(true);
    }
    else
        _status->setS(true);
}

void MOS6502CPU::CPY1()
{
    CPY(_memory->read(_programCounter++));
}

void MOS6502CPU::CPY3()
{
    CPY(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::CPY2()
{
    unsigned short address = getAbsolute();
    CPY(_memory->read(address));
}

void MOS6502CPU::DEC(unsigned short address)
{
    //locals
    byte value = _memory->read(address);

    if(value > 0)
        value--;
    else //its zero, so make it #FF
        value = 0xFF;

    _status->setS(value > NEGATIVE);
    _status->setZ(value == 0);

    //save value back to memory
    _memory->write(value, address);
}

void MOS6502CPU::DEC3()
{
    DEC(_memory->read(_programCounter++));
}

void MOS6502CPU::DEC7()
{
    unsigned short address = getZeroPageIndexed(_x);
    DEC(address);
}

void MOS6502CPU::DEC2()
{
    unsigned short address = getAbsolute();
    DEC(address);
}

void MOS6502CPU::DEC6()
{
    unsigned short address = getAbsolute();
    DEC(address + _x);
}

void MOS6502CPU::DEX4()
{
    if(_x == 0)
    {
        _x = 255;
        _status->setS(true);
    }
    else
    {
        _x--;

        _status->setS(_x > NEGATIVE);
        _status->setZ(_x == 0);
    }
}

void MOS6502CPU::DEY4()
{
    if(_y == 0)
    {
        _y = 255;
        _status->setS(true);
    }
    else
    {
        _y--;

        _status->setS(_y > NEGATIVE);
        _status->setZ(_y == 0);
    }
}

void MOS6502CPU::EOR(byte operand)
{
    _accumulator ^= operand;

    //set status
    _status->setZ(_accumulator == 0);
    _status->setS(_accumulator > NEGATIVE);
}

void MOS6502CPU::EOR1()
{
    EOR(_memory->read(_programCounter++));
}

void MOS6502CPU::EOR3()
{
    EOR(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::EOR7()
{
    unsigned short address = getZeroPageIndexed(_x);
    EOR(_memory->read(address));
}

void MOS6502CPU::EOR2()
{
    unsigned short address = getAbsolute();
    EOR(_memory->read(address));
}

void MOS6502CPU::EOR6_X()
{
    unsigned short address = getAbsolute();
    EOR(_memory->read(address + _x));
}

void MOS6502CPU::EOR6_Y()
{
    unsigned short address = getAbsolute();
    EOR(_memory->read(address + _y));
}

void MOS6502CPU::EOR9()
{
    unsigned short address = getPreIndirect();
    EOR(_memory->read(address));
}

void MOS6502CPU::EOR10()
{
    unsigned short address = getPostIndirect();
    EOR(_memory->read(address));
}

void MOS6502CPU::INC(unsigned short address)
{
    //locals
    byte value = _memory->read(address);

    if(value == 0xFF) //incrementing 255 would make 0
        value = 0x00;
    else
        value++;

    _status->setS(value > NEGATIVE);
    _status->setZ(value == 0);

    //save value back to memory
    _memory->write(value, address);
}

void MOS6502CPU::INC3()
{
    INC(_memory->read(_programCounter++));
}

void MOS6502CPU::INC7()
{
    unsigned short address = getZeroPageIndexed(_x);
    INC(address);
}

void MOS6502CPU::INC2()
{
    unsigned short address = getAbsolute();
    INC(address);
}

void MOS6502CPU::INC6()
{
    unsigned short address = getAbsolute();
    INC(address + _x);
}

void MOS6502CPU::INX4()
{
    if(_x == 255)
    {
        _x = 0;
        _status->setZ(true);
    }
    else
    {
        _x++;
        _status->setS(_x > NEGATIVE);
        _status->setZ(_x == 0);
    }
}

void MOS6502CPU::INY4()
{
    if(_y == 255)
    {
        _y = 0;
        _status->setZ(true);
    }
    else
    {
        _y++;
        _status->setS(_y > NEGATIVE);
        _status->setZ(_y == 0);
    }
}

void MOS6502CPU::JMP2()
{
    _programCounter = getAbsolute();
}

void MOS6502CPU::JMP8() //only operation that uses 8
{
    /*JMP8(indirect) composes a 16bit address just like JMP2,
     *but the 16bit address obtained is the start address of yet another
     *composite address.
     *SO: Get absolute on current position, will get you the 16bit address for obtaining the real address.
     *Then move to that start address and call getAbsolute again to retreive the REAL address.
     */
    for(byte i = 0; i < 2; i++)
        _programCounter = getAbsolute();
}

void MOS6502CPU::JSR2()
{
    //locals
    unsigned short address = getAbsolute();

    //save program counter
    _programCounter--;
    saveProgramCounter();
    _programCounter = address;
}

void MOS6502CPU::LDA(byte operand)
{
    //set status
    _status->setS(operand > NEGATIVE);
    _status->setZ(operand == 0);

    _accumulator = operand;
}

void MOS6502CPU::LDA1()
{
    LDA(_memory->read(_programCounter++));
}

void MOS6502CPU::LDA3()
{
    LDA(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::LDA7()
{
    unsigned short address = getZeroPageIndexed(_x);
    LDA(_memory->read(address));
}

void MOS6502CPU::LDA2()
{
    unsigned short address = getAbsolute();
    LDA(_memory->read(address));
}

void MOS6502CPU::LDA6_X()
{
    unsigned short address = getAbsolute();
    LDA(_memory->read(address + _x));
}

void MOS6502CPU::LDA6_Y()
{
    unsigned short address = getAbsolute();
    LDA(_memory->read(address + _y));
}

void MOS6502CPU::LDA9()
{
    unsigned short address = getPreIndirect();
    LDA(_memory->read(address));
}

void MOS6502CPU::LDA10()
{
    unsigned short address = getPostIndirect();
    LDA(_memory->read(address));
}

void MOS6502CPU::LDX(byte operand)
{
    //set status
    _status->setS(operand > NEGATIVE);
    _status->setZ(operand == 0);

    _x = operand;
}

void MOS6502CPU::LDX1()
{
    LDX(_memory->read(_programCounter++));
}

void MOS6502CPU::LDX3()
{
    LDX(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::LDX7()
{
    unsigned short address = getZeroPageIndexed(_y);
    LDX(_memory->read(address));
}

void MOS6502CPU::LDX2()
{
    unsigned short address = getAbsolute();
    LDX(_memory->read(address));
}

void MOS6502CPU::LDX6()
{
    unsigned short address = getAbsolute();
    LDX(_memory->read(address + _y));
}

void MOS6502CPU::LDY(byte operand)
{
    //set status
    _status->setS(operand > NEGATIVE);
    _status->setZ(operand == 0);

    _y = operand;
}

void MOS6502CPU::LDY1()
{
    LDY(_memory->read(_programCounter++));
}

void MOS6502CPU::LDY3()
{
    LDY(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::LDY7()
{
    unsigned short address = getZeroPageIndexed(_x);
    LDY(_memory->read(address));
}

void MOS6502CPU::LDY2()
{
    unsigned short address = getAbsolute();
    LDY(_memory->read(address));
}

void MOS6502CPU::LDY6()
{
    unsigned short address = getAbsolute();
    LDY(_memory->read(address + _x));
}

byte MOS6502CPU::LSR(byte operand)
{
    byte value = operand >> 1;

    _status->setZ(value == 0);
    _status->setS(false); //will always be false as we're effectively making bit 7 a zero

    //bit that represents '1' is used to determine carry, if the original value(operand) is odd then carry will be set.
    _status->setC((operand % 2) > 0);

    return value;
}

void MOS6502CPU::LSR5()
{
    _accumulator = LSR(_accumulator);
}

void MOS6502CPU::LSR3()
{
    unsigned short address = _memory->read(_programCounter++);
    _memory->write(LSR(_memory->read(address)), address);
}

void MOS6502CPU::LSR7()
{
    unsigned short address = getZeroPageIndexed(_x);
    _memory->write(LSR(_memory->read(address)), address);
}

void MOS6502CPU::LSR2()
{
    unsigned short address = getAbsolute();
    _memory->write(LSR(_memory->read(address)), address);
}

void MOS6502CPU::LSR6()
{
    unsigned short address = getAbsolute() + _x;
    _memory->write(LSR(_memory->read(address)), address);
}

void MOS6502CPU::ORA(byte operand)
{
    _accumulator |= operand;

    //set status
    _status->setZ(_accumulator == 0);
    _status->setS(_accumulator > NEGATIVE);
}

void MOS6502CPU::ORA1()
{
    ORA(_memory->read(_programCounter++));
}

void MOS6502CPU::ORA3()
{
    ORA(_memory->read(_memory->read(_programCounter++)));
}

void MOS6502CPU::ORA7()
{
    unsigned short address = getZeroPageIndexed(_x);
    ORA(_memory->read(address));
}

void MOS6502CPU::ORA2()
{
    unsigned short address = getAbsolute();
    ORA(_memory->read(address));
}

void MOS6502CPU::ORA6_X()
{
    unsigned short address = getAbsolute();
    ORA(_memory->read(address + _x));
}

void MOS6502CPU::ORA6_Y()
{
    unsigned short address = getAbsolute();
    ORA(_memory->read(address + _y));
}

void MOS6502CPU::ORA9()
{
    unsigned short address = getPreIndirect();
    ORA(_memory->read(address));
}

void MOS6502CPU::ORA10()
{
    unsigned short address = getPostIndirect();
    ORA(_memory->read(address));
}

void MOS6502CPU::PHA4()
{
    _stack->push(_accumulator);
}

void MOS6502CPU::PHP4()
{
    _stack->push(_status->toByte());
}

void MOS6502CPU::PLA4()
{
    _accumulator = _stack->pop();
    _status->setZ(_accumulator == 0);
    _status->setS(_accumulator > NEGATIVE);
}

//Return from JSR2
void MOS6502CPU::RTS4()
{
    getProgramCounter();
    _programCounter++;
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
    case 0x00: BRK4(); break;
    case 0x01: ORA9(); break;
    case 0x05: ORA3(); break;
    case 0x06: ASL3(); break;
    case 0x09: ORA1(); break;
    case 0x0A: ASL5(); break;
    case 0x0D: ORA2(); break;
    case 0x0E: ASL2(); break;
    case 0x08: PHP4(); break;
    case 0x10: BPL11(); break;
    case 0x11: ORA10(); break;
    case 0x15: ORA7(); break;
    case 0x16: ASL7(); break;
    case 0x18: CLC4(); break;
    case 0x19: ORA6_Y(); break;
    case 0x1D: ORA6_X(); break;
    case 0x1E: ASL6(); break;
    case 0x20: JSR2(); break;
    case 0x21: AND9(); break;
    case 0x24: BIT3(); break;
    case 0x25: AND3(); break;
    case 0x29: AND1(); break;
    case 0x2C: BIT2(); break;
    case 0x2D: AND2(); break;
    case 0x30: BMI11(); break;
    case 0x31: AND10(); break;
    case 0x35: AND7(); break;
    case 0x39: AND6_Y(); break;
    case 0x3D: AND6_X(); break;
    case 0x41: EOR9(); break;
    case 0x45: EOR3(); break;
    case 0x46: LSR3(); break;
    case 0x48: PHA4(); break;
    case 0x49: EOR1(); break;
    case 0x4A: LSR5(); break;
    case 0x4C: JMP2(); break;
    case 0x4D: EOR2(); break;
    case 0x4E: LSR2(); break;
    case 0x50: BVC11(); break;
    case 0x51: EOR10(); break;
    case 0x55: EOR7(); break;
    case 0x56: LSR7(); break;
    case 0x58: CLI4(); break;
    case 0x59: EOR6_Y(); break;
    case 0x5D: EOR6_X(); break;
    case 0x5E: LSR6(); break;
    case 0x60: RTS4(); break;
    case 0x61: ADC9(); break;
    case 0x65: ADC3(); break;
    case 0x68: PLA4(); break;
    case 0x69: ADC1(); break;
    case 0x6C: JMP8(); break;
    case 0x6D: ADC2(); break;
    case 0x70: BVS11(); break;
    case 0x71: ADC10(); break;
    case 0x75: ADC7(); break;
    case 0x79: ADC6_Y(); break;
    case 0x7D: ADC6_X(); break;
    case 0x88: DEY4(); break;
    case 0x90: BCC11(); break;
    case 0xA0: LDY1(); break;
    case 0xA1: LDA9(); break;
    case 0xA2: LDX1(); break;
    case 0xA4: LDY3(); break;
    case 0xA5: LDA3(); break;
    case 0xA6: LDX3(); break;
    case 0xA9: LDA1(); break;
    case 0xAC: LDY2(); break;
    case 0xAD: LDA2(); break;
    case 0xAE: LDX2(); break;
    case 0xB0: BCS11(); break;
    case 0xB1: LDA10(); break;
    case 0xB4: LDY7(); break;
    case 0xB5: LDA7(); break;
    case 0xB6: LDX7(); break;
    case 0xB8: CLV4(); break;
    case 0xB9: LDA6_Y(); break;
    case 0xBC: LDY6(); break;
    case 0xBD: LDA6_X(); break;
    case 0xBE: LDX6(); break;
    case 0xC0: CPY1(); break;
    case 0xC1: CMP9(); break;
    case 0xC4: CPY3(); break;
    case 0xC5: CMP3(); break;
    case 0xC6: DEC3(); break;
    case 0xC8: INY4(); break;
    case 0xC9: CMP1(); break;
    case 0xCA: DEX4(); break;
    case 0xCC: CPY2(); break;
    case 0xCD: CMP2(); break;
    case 0xCE: DEC2(); break;
    case 0xD0: BNE11(); break;
    case 0xD1: CMP10(); break;
    case 0xD5: CMP7(); break;
    case 0xD6: DEC7(); break;
    case 0xD8: CLD4(); break;
    case 0xD9: CMP6_Y(); break;
    case 0xDD: CMP6_X(); break;
    case 0xDE: DEC6(); break;
    case 0xE0: CPX1(); break;
    case 0xE4: CPX3(); break;
    case 0xE6: INC3(); break;
    case 0xE8: INX4(); break;
    case 0xEA: return; //NOP command, do nothing
    case 0xEC: CPX2(); break;
    case 0xEE: INC2(); break;
    case 0xF0: BEQ11(); break;
    case 0xF6: INC7(); break;
    case 0xFE: INC6(); break;

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
    _stack->reset();
    _accumulator = 0;
    _x = 0;
    _y = 0;
    _programCounter = 0;
    _stackPointer = 0xFF;
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
    status("STATUS");
}

void MOS6502CPU::status(string header)
{
    //Locals
    byte stackSize = _stack->size();

    cout << "\n======================" << header << "======================" << endl;
    cout << "Registers:" << endl;
    cout << "PC: 0x" << hex << _programCounter << endl;
    cout << "AC: 0x" << hex << (int)_accumulator << endl; //int cast because its interpreting it as char, not a number
    cout << "XR: 0x" << hex << (int)_x << endl;
    cout << "YR: 0x" << hex << (int)_y << endl;
    cout << "SP: 0x" << hex << (int)_stackPointer << endl << endl;

    cout << "Status Register:" << endl;
    cout << "|S|V| |B|D|I|Z|C|" << endl;
    cout << "|" << _status->getS();
    cout << "|" << _status->getV();
    cout << "|1";
    cout << "|" << _status->getB();
    cout << "|" << _status->getD();
    cout << "|" << _status->getI();
    cout << "|" << _status->getZ();
    cout << "|" << _status->getC() << endl << endl;

    cout << "Stack:" << endl;
    cout << "Size: " << (int)stackSize << endl;

    if(stackSize > 0)
    {
        cout << "Contents:" << endl;

        /*TODO:
         *Print stack contents here, make a copy of the current stack and then pop the elements.
         *Make it view like this:
         *  [Top] = <element>
         *  [1] = <element>
         *  ...
         *  [n] = <element>
         */

        Stack copy = *_stack;

        for(int i = 0; i < stackSize; i++)
        {
            if(i == 0)
                cout << "[Top] = 0x" << hex << (int)copy[i] << endl;
            else
                cout << "[" << i << "] = 0x" << hex << (int)copy[i] << endl;
        }
    }

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
    _stack = new Stack(this);
    _running = false;
    _debug = debug;

    //Setup registers
    _status = new StatusRegister();
    _accumulator = 0;
    _x = 0;
    _y = 0;
    _programCounter = 0;
    _stackPointer = 0xFF;
}

//Destructor
MOS6502CPU::~MOS6502CPU()
{
    delete _status;
    delete _memory;
    delete _stack;
}
