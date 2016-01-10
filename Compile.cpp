//Compile class implementation
#include "Compiler.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "Exceptions.h"

using namespace MOS_6502;
using namespace std;

//--General(private)
string Compiler::fetchCommand(string& line)
{
    //TODO: fetch first letter, use letter to access bucket, iterate through bucket and return the command. Exception if none found
    //This assumes the line has been trimmed
    char first = toupper(line[0]);

    auto i = _commands.find(first);

    if(i != _commands.end())
    {
        vector<string> bucket = i->second;

        for(vector<string>::iterator b = bucket.begin(); b < bucket.end(); b++)
        {
            if(regex_search(line, regex(*b))) //TODO: everything works fine, regex is breaking.
                return *b;
        }
    }

	throw new CompilerException(_state.lineNo, string("Unknown Command"));
}

string Compiler::stripComments(string& line)
{
	unsigned int i;
	bool found = false;

	//trim comments
	for(i = 0; i < line.length(); i++)
	{
		if(line[i] == ';')
		{
			found = true;
			break;
		}
	}

	if(found)
		line = line.substr(0, i);

    //trim spaces
    unsigned int l, r;

    for(l = 0; l < line.length(); l++)
    {
        if(line[l] != ' ')
            break;
    }

    for(r = line.length()-1; r > 0; r--)
    {
        if(line[r] != ' ')
            break;
    }

    if(r >= l)
        line = line.substr(l, (r-l)+1);

	return line;
}

vector<byte> Compiler::compileLine(string& line)
{
	cout << line << endl; //TODO: TEMP

	vector<byte> oppcodes;

	if(line.empty())
        return oppcodes;

	//strip comments from line
	line = stripComments(line);

	cout << "stripped: " << line << endl;

	/* TODO:
	 * Have a list of valid commands
	 * extract command from line
	 * check command agaisnt list, if none then do a compiler error
	 * check command agaisnt the addressing mode regexes, if none then compiler error.
	 * return oppcodes
	 */

	string command;

	try
	{
		command = fetchCommand(line);
	}
	catch (CompilerException* e)
	{
		throw e;
	}

    cout << "command: " << command << endl;

	return oppcodes;
}

void Compiler::setupCommands()
{
    _commands['A'] = vector<string>();
    vector<string>* a = &_commands['A'];

    a->push_back("ADC");
    a->push_back("AND");
    a->push_back("ASL");

    _commands['B'] = vector<string>();
    vector<string>* b = &_commands['B'];

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

    _commands['C'] = vector<string>();
    vector<string>* c = &_commands['C'];

    c->push_back("CLC");
    c->push_back("CLD");
    c->push_back("CLI");
    c->push_back("CLV");
    c->push_back("CMP");
    c->push_back("CPX");
    c->push_back("CPY");

    _commands['D'] = vector<string>();
    vector<string>* d = &_commands['D'];

    d->push_back("DEC");
    d->push_back("DEX");
    d->push_back("DEY");

    _commands['E'] = vector<string>();
    vector<string>* e = &_commands['E'];

    e->push_back("EOR");

    _commands['I'] = vector<string>();
    vector<string>* i = &_commands['I'];

    i->push_back("INC");
    i->push_back("INX");
    i->push_back("INY");

    _commands['J'] = vector<string>();
    vector<string>* j = &_commands['J'];

    j->push_back("JMP");
    j->push_back("JSR");

    _commands['L'] = vector<string>();
    vector<string>* l = &_commands['L'];

    l->push_back("LDA");
    l->push_back("LDX");
    l->push_back("LDY");
    l->push_back("LSR");

    _commands['N'] = vector<string>();
    vector<string>* n = &_commands['N'];

    n->push_back("NOP");

    _commands['O'] = vector<string>();
    vector<string>* o = &_commands['O'];

    o->push_back("ORA");

    _commands['P'] = vector<string>();
    vector<string>* p = &_commands['P'];

    p->push_back("PHA");
    p->push_back("PHP");
    p->push_back("PLA");
    p->push_back("PLP");

    _commands['R'] = vector<string>();
    vector<string>* r = &_commands['R'];

    r->push_back("ROL");
    r->push_back("ROR");
    r->push_back("RTI");
    r->push_back("RTS");

    _commands['S'] = vector<string>();
    vector<string>* s = &_commands['S'];

    s->push_back("SBC");
    s->push_back("SEC");
    s->push_back("SED");
    s->push_back("SEI");
    s->push_back("STA");
    s->push_back("STX");
    s->push_back("STY");

    _commands['T'] = vector<string>();
    vector<string>* t = &_commands['T'];

    t->push_back("TAX");
    t->push_back("TAY");
    t->push_back("TSX");
    t->push_back("TXA");
    t->push_back("TXS");
    t->push_back("TYA");

}

void Compiler::resetState()
{
	_state.lineNo = 0;
}

//--General(public)
vector<byte> Compiler::compile(string src)
{
	//TODO: split src into lines, call compileLine on each
    cout << src << endl; //TODO: TEMP
	return vector<byte>();
}

vector<byte> Compiler::compileFromFile(string filePath)
{
    ifstream io;
    io.open(filePath, ifstream::in);

    if(io.fail())
    {
		throw new exception(("Could not open file \"" + filePath + "\"").c_str());
    }

	resetState();

	vector<byte> program;

	if(io.is_open())
	{
		while(io.good())
		{
			string line;

			//read and compile each line
			getline(io, line);

			try 
			{
				vector<byte> oppcodes = compileLine(line);

				for (vector<byte>::iterator i = oppcodes.begin(); i < oppcodes.end(); i++)
				{
					program.insert(program.end(), *i);
				}
			}
			catch(CompilerException* e)
			{
				throw e;
			}

			_state.lineNo++;
		}
	}

    io.close();
	return program;
}

//--Constructor
Compiler::Compiler()
{
    setupCommands();
}
