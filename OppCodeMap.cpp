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
	vector<byte>* and = &this->at("AND");

	(*and)[AddressingModes::IMMEDIATE] = 0x29;
	(*and)[AddressingModes::ZEROPAGE] = 0x25;
	(*and)[AddressingModes::ZEROPAGE] = 0x35;
	(*and)[AddressingModes::ABSOLUTE] = 0x2D;
	(*and)[AddressingModes::INDEXED_X] = 0x3D;
	(*and)[AddressingModes::INDEXED_Y] = 0x39;
	(*and)[AddressingModes::PRE_INDEXED_INDIRECT] = 0x21;
	(*and)[AddressingModes::POST_INDEXED_INDIRECT] = 0x31;

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

}
