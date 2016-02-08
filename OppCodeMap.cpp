#include "OppCodeMap.h"

using namespace std;
using namespace MOS_6502::CompilerAssets;

//--Constants
const byte OppCodeMap::ADDRESSING_MODES = 12;
const byte OppCodeMap::INVALID_MODE = 0xFF;

//--Constructor
OppCodeMap::OppCodeMap()
{
	this->insert(pair<string, vector<byte>>("ADC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* adc = &this->at("ADC");

	(*adc)[AddressingModes::IMMEDIATE] = 0x69;
	(*adc)[AddressingModes::ZEROPAGE] = 0x65;
	(*adc)[AddressingModes::ZEROPAGE_INDEXED] = 0x75;
	(*adc)[AddressingModes::ABSOLUTE] = 0x6D;
	(*adc)[AddressingModes::INDEXED_X] = 0x7D;
	(*adc)[AddressingModes::INDEXED_Y] = 0x79;
	(*adc)[AddressingModes::PRE_INDEXED_INDIRECT] = 0x61;
	(*adc)[AddressingModes::POST_INDEXED_INDIRECT] = 0x71;

	this->insert(pair<string, vector<byte>>("AND", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* and_c = &this->at("AND");

	(*and_c)[AddressingModes::IMMEDIATE] = 0x29;
	(*and_c)[AddressingModes::ZEROPAGE] = 0x25;
	(*and_c)[AddressingModes::ZEROPAGE] = 0x35;
	(*and_c)[AddressingModes::ABSOLUTE] = 0x2D;
	(*and_c)[AddressingModes::INDEXED_X] = 0x3D;
	(*and_c)[AddressingModes::INDEXED_Y] = 0x39;
	(*and_c)[AddressingModes::PRE_INDEXED_INDIRECT] = 0x21;
	(*and_c)[AddressingModes::POST_INDEXED_INDIRECT] = 0x31;

	this->insert(pair<string, vector<byte>>("ASL", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* asl = &this->at("ASL");

	(*asl)[AddressingModes::ACCUMULATOR] = 0x0A;
	(*asl)[AddressingModes::ZEROPAGE] = 0x06;
	(*asl)[AddressingModes::ZEROPAGE_INDEXED] = 0x16;
	(*asl)[AddressingModes::ABSOLUTE] = 0x0E;
	(*asl)[AddressingModes::INDEXED_X] = 0x1E;

	this->insert(pair<string, vector<byte>>("BCC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bcc = &this->at("BCC");

	(*bcc)[AddressingModes::RELATIVE] = 0x90;

	this->insert(pair<string, vector<byte>>("BCS", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bcs = &this->at("BCS");

	(*bcs)[AddressingModes::RELATIVE] = 0xB0;

	this->insert(pair<string, vector<byte>>("BEQ", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* beq = &this->at("BEQ");

	(*beq)[AddressingModes::RELATIVE] = 0xF0;

	this->insert(pair<string, vector<byte>>("BIT", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bit = &this->at("BIT");

	(*bit)[AddressingModes::ZEROPAGE] = 0x24;
	(*bit)[AddressingModes::ABSOLUTE] = 0x2C;

	this->insert(pair<string, vector<byte>>("BMI", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bmi = &this->at("BMI");

	(*bmi)[AddressingModes::RELATIVE] = 0x30;

	this->insert(pair<string, vector<byte>>("BNE", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bne = &this->at("BNE");

	(*bne)[AddressingModes::RELATIVE] = 0xD0;

	this->insert(pair<string, vector<byte>>("BPL", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bpl = &this->at("BPL");

	(*bpl)[AddressingModes::RELATIVE] = 0x10;

	this->insert(pair<string, vector<byte>>("BRK", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* brk = &this->at("BRK");

	(*brk)[AddressingModes::IMPLIED] = 0x00;

	this->insert(pair<string, vector<byte>>("BVC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bvc = &this->at("BVC");

	(*bvc)[AddressingModes::RELATIVE] = 0x50;

    this->insert(pair<string, vector<byte>>("BVS", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* bvs = &this->at("BVS");

	(*bvs)[AddressingModes::RELATIVE] = 0x70;

	this->insert(pair<string, vector<byte>>("CLC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* clc = &this->at("CLC");

	(*clc)[AddressingModes::IMPLIED] = 0x18;

	this->insert(pair<string, vector<byte>>("CLD", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* cld = &this->at("CLD");

	(*cld)[AddressingModes::IMPLIED] = 0xD8;

	this->insert(pair<string, vector<byte>>("CLI", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* cli = &this->at("CLI");

	(*cli)[AddressingModes::IMPLIED] = 0x58;

	this->insert(pair<string, vector<byte>>("CLV", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* clv = &this->at("CLV");

	(*clv)[AddressingModes::IMPLIED] = 0xB8;

	this->insert(pair<string, vector<byte>>("CMP", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* cmp = &this->at("CMP");

	(*cmp)[AddressingModes::IMMEDIATE] = 0xC9;
	(*cmp)[AddressingModes::ZEROPAGE] = 0xC5;
	(*cmp)[AddressingModes::ZEROPAGE_INDEXED] = 0xD5;
	(*cmp)[AddressingModes::ABSOLUTE] = 0xCD;
    (*cmp)[AddressingModes::INDEXED_X] = 0xDD;
    (*cmp)[AddressingModes::INDEXED_Y] = 0xD9;
    (*cmp)[AddressingModes::PRE_INDEXED_INDIRECT] = 0xC1;
    (*cmp)[AddressingModes::POST_INDEXED_INDIRECT] = 0xD1;

    this->insert(pair<string, vector<byte>>("CPX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* cpx = &this->at("CPX");

	(*cpx)[AddressingModes::IMMEDIATE] = 0xE0;
	(*cpx)[AddressingModes::ZEROPAGE] = 0xE4;
	(*cpx)[AddressingModes::ABSOLUTE] = 0xEC;

	this->insert(pair<string, vector<byte>>("CPY", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* cpy = &this->at("CPY");

	(*cpy)[AddressingModes::IMMEDIATE] = 0xC0;
	(*cpy)[AddressingModes::ZEROPAGE] = 0xC4;
	(*cpy)[AddressingModes::ABSOLUTE] = 0xCC;

	this->insert(pair<string, vector<byte>>("DEC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* dec = &this->at("DEC");

	(*dec)[AddressingModes::ZEROPAGE] = 0xC6;
	(*dec)[AddressingModes::ZEROPAGE_INDEXED] = 0xD6;
	(*dec)[AddressingModes::ABSOLUTE] = 0xCE;
	(*dec)[AddressingModes::INDEXED_X] = 0xDE;

	this->insert(pair<string, vector<byte>>("DEX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* dex = &this->at("DEX");

	(*dex)[AddressingModes::IMPLIED] = 0xCA;

	this->insert(pair<string, vector<byte>>("DEY", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* dey = &this->at("DEY");

	(*dey)[AddressingModes::IMPLIED] = 0x88;

	this->insert(pair<string, vector<byte>>("EOR", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* eor = &this->at("EOR");

	(*eor)[AddressingModes::IMMEDIATE] = 0x49;
	(*eor)[AddressingModes::ZEROPAGE] = 0x45;
	(*eor)[AddressingModes::ZEROPAGE_INDEXED] = 0x55;
	(*eor)[AddressingModes::ABSOLUTE] = 0x4D;
	(*eor)[AddressingModes::INDEXED_X] = 0x5D;
	(*eor)[AddressingModes::INDEXED_Y] = 0x59;
	(*eor)[AddressingModes::PRE_INDEXED_INDIRECT] = 0x41;
	(*eor)[AddressingModes::POST_INDEXED_INDIRECT] = 0x51;

	this->insert(pair<string, vector<byte>>("INC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* inc = &this->at("INC");

	(*inc)[AddressingModes::ZEROPAGE] = 0xE6;
	(*inc)[AddressingModes::ZEROPAGE_INDEXED] = 0xF6;
	(*inc)[AddressingModes::ABSOLUTE] = 0xEE;
	(*inc)[AddressingModes::INDEXED_X] = 0xFE;

}
