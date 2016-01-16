//Compile class implementation
#include "Compiler.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "Exceptions.h"
#include "CommandMap.h"

using namespace MOS_6502;
using namespace MOS_6502::CompilerAssets;
using namespace std;

//--Constants
const byte Compiler::ADDRESSING_MODES = 12;
const byte Compiler::INVALID_MODE = 0;

//--General(private)
string Compiler::fetchCommand(string& line)
{
    //TODO: fetch first letter, use letter to access bucket, iterate through bucket and return the command. Exception if none found
    //This assumes the line has been trimmed
    char first = toupper(line[0]);

    auto i = _commands->find(first);

    if(i != _commands->end())
    {
        vector<string> bucket = i->second;

        for(vector<string>::iterator b = bucket.begin(); b < bucket.end(); b++)
        {
            if(regex_search(line, regex(*b)))
                return *b;
        }
    }

	throw new CompilerException(_state.lineNo, string("Unknown Command"));
}

vector<byte> Compiler::fetchOppcodes(string& command, string& line)
{
	return vector<byte>();
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
	if(line.empty())
        return vector<byte>();

	//strip comments from line
	line = stripComments(line);

	try
	{
		string command = fetchCommand(line);
		vector<byte> oppcodes = fetchOppcodes(command, line);

		return oppcodes;
	}
	catch (CompilerException* e)
	{
		throw e;
	}
}

void Compiler::setupOppcodes()
{
	_oppcodes["ADC"] = vector<byte>(ADDRESSING_MODES, INVALID_MODE);
	_oppcodes["ADC"][AddressingModes::IMMEDIATE] = 0x69;
	_oppcodes["ADC"][AddressingModes::ZEROPAGE] = 0x65;
	_oppcodes["ADC"][AddressingModes::ZEROPAGE_INDEXED] = 0x75;
	_oppcodes["ADC"][AddressingModes::ABSOLUTE] = 0x6D;
	_oppcodes["ADC"][AddressingModes::INDEXED_X] = 0x7D;
	_oppcodes["ADC"][AddressingModes::INDEXED_Y] = 0x79;
	_oppcodes["ADC"][AddressingModes::PRE_INDEXED_INDIRECT] = 0x61;
	_oppcodes["ADC"][AddressingModes::POST_INDEXED_INDIRECT] = 0x71;

	_oppcodes["AND"] = vector<byte>(ADDRESSING_MODES, INVALID_MODE);
	_oppcodes["AND"][AddressingModes::IMMEDIATE] = 0x29;
	_oppcodes["AND"][AddressingModes::ZEROPAGE] = 0x25;
	_oppcodes["AND"][AddressingModes::ZEROPAGE] = 0x35;
	_oppcodes["AND"][AddressingModes::ABSOLUTE] = 0x2D;
	_oppcodes["AND"][AddressingModes::INDEXED_X] = 0x3D;
	_oppcodes["AND"][AddressingModes::INDEXED_Y] = 0x39;
	_oppcodes["AND"][AddressingModes::PRE_INDEXED_INDIRECT] = 0x21;
	_oppcodes["AND"][AddressingModes::POST_INDEXED_INDIRECT] = 0x31;
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
		throw new CompilerException(string("Could not open file \"" + filePath + "\""));
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
			    io.close();
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
    _commands = new CommandMap();
	setupOppcodes();
}

//--Destructor
Compiler::~Compiler()
{
    delete _commands;
}