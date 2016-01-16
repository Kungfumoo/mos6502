#include "OppCodeMap.h"

using namespace std;
using namespace MOS_6502::CompilerAssets;

//--Constants
const byte OppCodeMap::ADDRESSING_MODES = 12;
const byte OppCodeMap::INVALID_MODE = 0;

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
}
