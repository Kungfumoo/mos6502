#include "OppCodeMap.h"
#include "Exceptions.h"

using namespace std;
using namespace MOS_6502;
using namespace MOS_6502::CompilerAssets;

//--Constants
const byte OppCodeMap::ADDRESSING_MODES = 12;
const byte OppCodeMap::INVALID_MODE = 0xFF;

//--Public Methods
byte OppCodeMap::fetchCommandCode(string cmd, AddressingModes mode)
{
    vector<byte> oppMap = at(cmd);
    byte value = oppMap[mode];

    if(value == INVALID_MODE)
        throw CompilerException("Invalid Addressing mode for CMD \"" + cmd + "\"");

    return value;
}

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

	this->insert(pair<string, vector<byte>>("INX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* inx = &this->at("INX");

	(*inx)[AddressingModes::IMPLIED] = 0xE8;

	this->insert(pair<string, vector<byte>>("INY", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* iny = &this->at("INY");

	(*iny)[AddressingModes::IMPLIED] = 0xC8;

	this->insert(pair<string, vector<byte>>("JMP", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* jmp = &this->at("JMP");

	(*jmp)[AddressingModes::ABSOLUTE] = 0x4C;
	(*jmp)[AddressingModes::INDIRECT] = 0x6C;

	this->insert(pair<string, vector<byte>>("JSR", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* jsr = &this->at("JSR");

	(*jsr)[AddressingModes::ABSOLUTE] = 0x20;

	this->insert(pair<string, vector<byte>>("LDA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* lda = &this->at("LDA");

	(*lda)[AddressingModes::IMMEDIATE] = 0xA9;
	(*lda)[AddressingModes::ZEROPAGE] = 0xA5;
	(*lda)[AddressingModes::ZEROPAGE_INDEXED] = 0xB5;
	(*lda)[AddressingModes::ABSOLUTE] = 0xAD;
	(*lda)[AddressingModes::INDEXED_X] = 0xBD;
	(*lda)[AddressingModes::INDEXED_Y] = 0xB9;
	(*lda)[AddressingModes::PRE_INDEXED_INDIRECT] = 0xA1;
	(*lda)[AddressingModes::POST_INDEXED_INDIRECT] = 0xB1;

	this->insert(pair<string, vector<byte>>("LDX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* ldx = &this->at("LDX");

	(*ldx)[AddressingModes::IMMEDIATE] = 0xA2;
	(*ldx)[AddressingModes::ZEROPAGE] = 0xA6;
	(*ldx)[AddressingModes::ZEROPAGE_INDEXED] = 0xB6; //to Y
	(*ldx)[AddressingModes::ABSOLUTE] = 0xAE;
	(*ldx)[AddressingModes::INDEXED_Y] = 0xBE;

	this->insert(pair<string, vector<byte>>("LDY", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* ldy = &this->at("LDY");

	(*ldy)[AddressingModes::IMMEDIATE] = 0xA0;
	(*ldy)[AddressingModes::ZEROPAGE] = 0xA4;
	(*ldy)[AddressingModes::ZEROPAGE_INDEXED] = 0xB4;
	(*ldy)[AddressingModes::ABSOLUTE] = 0xAC;
	(*ldy)[AddressingModes::INDEXED_X] = 0xBC;

	this->insert(pair<string, vector<byte>>("LSR", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* lsr = &this->at("LSR");

	(*lsr)[AddressingModes::ACCUMULATOR] = 0x4A;
	(*lsr)[AddressingModes::ZEROPAGE] = 0x46;
	(*lsr)[AddressingModes::ZEROPAGE_INDEXED] = 0x56;
	(*lsr)[AddressingModes::ABSOLUTE] = 0x4E;
	(*lsr)[AddressingModes::INDEXED_X] = 0x5E;

	this->insert(pair<string, vector<byte>>("NOP", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* nop = &this->at("NOP");

	(*nop)[AddressingModes::IMPLIED] = 0xEA;

	this->insert(pair<string, vector<byte>>("ORA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* ora = &this->at("ORA");

	(*ora)[AddressingModes::IMMEDIATE] = 0x09;
	(*ora)[AddressingModes::ZEROPAGE] = 0x05;
	(*ora)[AddressingModes::ZEROPAGE_INDEXED] = 0x15;
	(*ora)[AddressingModes::ABSOLUTE] = 0x0D;
	(*ora)[AddressingModes::INDEXED_X] = 0x1D;
	(*ora)[AddressingModes::INDEXED_Y] = 0x19;
	(*ora)[AddressingModes::PRE_INDEXED_INDIRECT] = 0x01;
	(*ora)[AddressingModes::POST_INDEXED_INDIRECT] = 0x11;

	this->insert(pair<string, vector<byte>>("PHA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* pha = &this->at("PHA");

	(*pha)[AddressingModes::IMPLIED] = 0x48;

	this->insert(pair<string, vector<byte>>("PHP", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* php = &this->at("PHP");

	(*php)[AddressingModes::IMPLIED] = 0x08;

	this->insert(pair<string, vector<byte>>("PLA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* pla = &this->at("PLA");

	(*pla)[AddressingModes::IMPLIED] = 0x68;

	this->insert(pair<string, vector<byte>>("PLP", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* plp = &this->at("PLP");

	(*plp)[AddressingModes::IMPLIED] = 0x28;

	this->insert(pair<string, vector<byte>>("ROL", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* rol = &this->at("ROL");

	(*rol)[AddressingModes::ACCUMULATOR] = 0x2A;
	(*rol)[AddressingModes::ZEROPAGE] = 0x26;
	(*rol)[AddressingModes::ZEROPAGE_INDEXED] = 0x36;
	(*rol)[AddressingModes::ABSOLUTE] = 0x2E;
	(*rol)[AddressingModes::INDEXED_X] = 0x3E;

	this->insert(pair<string, vector<byte>>("ROR", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* ror = &this->at("ROR");

	(*ror)[AddressingModes::ACCUMULATOR] = 0x6A;
	(*ror)[AddressingModes::ZEROPAGE] = 0x66;
	(*ror)[AddressingModes::ZEROPAGE_INDEXED] = 0x76;
	(*ror)[AddressingModes::ABSOLUTE] = 0x6E;
	(*ror)[AddressingModes::INDEXED_X] = 0x7E;

	this->insert(pair<string, vector<byte>>("RTI", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* rti = &this->at("RTI");

	(*rti)[AddressingModes::IMPLIED] = 0x40;

	this->insert(pair<string, vector<byte>>("RTS", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* rts = &this->at("RTS");

	(*rts)[AddressingModes::IMPLIED] = 0x60;

	this->insert(pair<string, vector<byte>>("SBC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* sbc = &this->at("SBC");

	(*sbc)[AddressingModes::IMMEDIATE] = 0xE9;
	(*sbc)[AddressingModes::ZEROPAGE] = 0xE5;
	(*sbc)[AddressingModes::ZEROPAGE_INDEXED] = 0xF5;
	(*sbc)[AddressingModes::ABSOLUTE] = 0xED;
	(*sbc)[AddressingModes::INDEXED_X] = 0xFD;
	(*sbc)[AddressingModes::INDEXED_Y] = 0xF9;
	(*sbc)[AddressingModes::PRE_INDEXED_INDIRECT] = 0xE1;
	(*sbc)[AddressingModes::POST_INDEXED_INDIRECT] = 0xF1;

	this->insert(pair<string, vector<byte>>("SEC", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* sec = &this->at("SEC");

	(*sec)[AddressingModes::IMPLIED] = 0x38;


	this->insert(pair<string, vector<byte>>("SED", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* sed = &this->at("SED");

	(*sed)[AddressingModes::IMPLIED] = 0xF8;

	this->insert(pair<string, vector<byte>>("SEI", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* sei = &this->at("SEI");

	(*sei)[AddressingModes::IMPLIED] = 0x78;


    this->insert(pair<string, vector<byte>>("STA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* sta = &this->at("STA");

	(*sta)[AddressingModes::ZEROPAGE] = 0x85;
	(*sta)[AddressingModes::ZEROPAGE_INDEXED] = 0x95;
	(*sta)[AddressingModes::ABSOLUTE] = 0x8D;
	(*sta)[AddressingModes::INDEXED_X] = 0x9D;
	(*sta)[AddressingModes::INDEXED_Y] = 0x99;
	(*sta)[AddressingModes::PRE_INDEXED_INDIRECT] = 0x81;
	(*sta)[AddressingModes::POST_INDEXED_INDIRECT] = 0x91;

	this->insert(pair<string, vector<byte>>("STX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* stx = &this->at("STX");

	(*stx)[AddressingModes::ZEROPAGE] = 0x86;
	(*stx)[AddressingModes::ZEROPAGE_INDEXED] = 0x96; //from Y though
	(*stx)[AddressingModes::ABSOLUTE] = 0x8E;

	this->insert(pair<string, vector<byte>>("STY", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* sty = &this->at("STY");

	(*sty)[AddressingModes::ZEROPAGE] = 0x84;
	(*sty)[AddressingModes::ZEROPAGE_INDEXED] = 0x94;
	(*sty)[AddressingModes::ABSOLUTE] = 0x8C;

	this->insert(pair<string, vector<byte>>("TAX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* tax = &this->at("TAX");

	(*tax)[AddressingModes::IMPLIED] = 0xAA;


    this->insert(pair<string, vector<byte>>("TAY", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* tay = &this->at("TAY");

	(*tay)[AddressingModes::IMPLIED] = 0xA8;

	this->insert(pair<string, vector<byte>>("TSX", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* tsx = &this->at("TSX");

	(*tsx)[AddressingModes::IMPLIED] = 0xBA;

	this->insert(pair<string, vector<byte>>("TXA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* txa = &this->at("TXA");

	(*txa)[AddressingModes::IMPLIED] = 0x8A;

	this->insert(pair<string, vector<byte>>("TXS", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* txs = &this->at("TXS");

	(*txs)[AddressingModes::IMPLIED] = 0x9A;

	this->insert(pair<string, vector<byte>>("TYA", vector<byte>(ADDRESSING_MODES, INVALID_MODE)));
	vector<byte>* tya = &this->at("TYA");

	(*tya)[AddressingModes::IMPLIED] = 0x98;
}
