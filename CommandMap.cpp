#include "CommandMap.h"

using namespace MOS_6502::CompilerAssets;
using namespace std;

//--Constructors
CommandMap::CommandMap() :
    unordered_map()
{
    this->insert(pair<char, vector<string>>('A', vector<string>()));
    vector<string>* a = &this->at('A');

    a->push_back("ADC");
    a->push_back("AND");
    a->push_back("ASL");

    this->insert(pair<char, vector<string>>('B', vector<string>()));
    vector<string>* b = &this->at('B');

    b->push_back("BCC");
    b->push_back("BCS");
    b->push_back("BEQ");
    b->push_back("BIT");
    b->push_back("BMI");
    b->push_back("BNE");
    b->push_back("BPL");
    b->push_back("BRK");
    b->push_back("BVC");
    b->push_back("BVS");

    this->insert(pair<char, vector<string>>('C', vector<string>()));
    vector<string>* c = &this->at('C');

    c->push_back("CLC");
    c->push_back("CLD");
    c->push_back("CLI");
    c->push_back("CLV");
    c->push_back("CMP");
    c->push_back("CPX");
    c->push_back("CPY");

    this->insert(pair<char, vector<string>>('D', vector<string>()));
    vector<string>* d = &this->at('D');

    d->push_back("DEC");
    d->push_back("DEX");
    d->push_back("DEY");

    this->insert(pair<char, vector<string>>('E', vector<string>()));
    vector<string>* e = &this->at('E');

    e->push_back("EOR");

    this->insert(pair<char, vector<string>>('I', vector<string>()));
    vector<string>* i = &this->at('I');

    i->push_back("INC");
    i->push_back("INX");
    i->push_back("INY");

    this->insert(pair<char, vector<string>>('J', vector<string>()));
    vector<string>* j = &this->at('J');

    j->push_back("JMP");
    j->push_back("JSR");

    this->insert(pair<char, vector<string>>('L', vector<string>()));
    vector<string>* l = &this->at('L');

    l->push_back("LDA");
    l->push_back("LDX");
    l->push_back("LDY");
    l->push_back("LSR");

    this->insert(pair<char, vector<string>>('N', vector<string>()));
    vector<string>* n = &this->at('N');

    n->push_back("NOP");

    this->insert(pair<char, vector<string>>('O', vector<string>()));
    vector<string>* o = &this->at('O');

    o->push_back("ORA");

    this->insert(pair<char, vector<string>>('P', vector<string>()));
    vector<string>* p = &this->at('P');

    p->push_back("PHA");
    p->push_back("PHP");
    p->push_back("PLA");
    p->push_back("PLP");

    this->insert(pair<char, vector<string>>('R', vector<string>()));
    vector<string>* r = &this->at('R');

    r->push_back("ROL");
    r->push_back("ROR");
    r->push_back("RTI");
    r->push_back("RTS");

    this->insert(pair<char, vector<string>>('S', vector<string>()));
    vector<string>* s = &this->at('S');

    s->push_back("SBC");
    s->push_back("SEC");
    s->push_back("SED");
    s->push_back("SEI");
    s->push_back("STA");
    s->push_back("STX");
    s->push_back("STY");

    this->insert(pair<char, vector<string>>('T', vector<string>()));
    vector<string>* t = &this->at('T');

    t->push_back("TAX");
    t->push_back("TAY");
    t->push_back("TSX");
    t->push_back("TXA");
    t->push_back("TXS");
    t->push_back("TYA");
}
