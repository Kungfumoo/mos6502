//Compile class implementation
#include "Compiler.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "Exceptions.h"
#include "CommandMap.h"
#include "OppCodeMap.h"

using namespace MOS_6502;
using namespace MOS_6502::CompilerAssets;
using namespace std;

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
    vector<byte> oppMap = this->_oppcodes->at(command);
    vector<byte> oppCodes;

    //work out addressing mode
    if(regex_search(line, regex("^[A-Z]{3}\ \#[0-9A-Za-z]{2}$"))) //1 immediate
    {
        oppCodes.push_back(oppMap[OppCodeMap::AddressingModes::IMMEDIATE]);
        oppCodes.push_back((byte)(stoi(line.substr(5), nullptr, 16)));
    }
    else if(regex_search(line, regex("^[A-Z]{3}\ [0-9A-Za-z]{4}$"))) //2 absolute
    {
        unsigned short operand = (unsigned short)stoi(line.substr(4), nullptr, 16);

        oppCodes.push_back(oppMap[OppCodeMap::AddressingModes::ABSOLUTE]);
        oppCodes.push_back((byte)((operand >> 8) & 255));
        oppCodes.push_back((byte)operand & 255);
    }
    else if(regex_search(line, regex("^[A-Z]{3}\ [0-9A-Za-z]{2}$"))) //3 zeropage
    {
        oppCodes.push_back(oppMap[OppCodeMap::AddressingModes::ZEROPAGE]);
        oppCodes.push_back((byte)(stoi(line.substr(4), nullptr, 16)));
    }
    else if(regex_search(line, regex("^[A-Z]{3}$"))) //4 implied
    {
        oppCodes.push_back(oppMap[OppCodeMap::AddressingModes::IMPLIED]);
    }
    else if(regex_search(line, regex("^[A-Z]{3}\ A$"))) //5 accumulator
    {
        oppCodes.push_back(oppMap[OppCodeMap::AddressingModes::ACCUMULATOR]);
    }
    else
    {
        //TODO: throw exception, unknown format
    }

	return oppCodes;
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
	_oppcodes = new OppCodeMap();
}

//--Destructor
Compiler::~Compiler()
{
    delete _commands;
	delete _oppcodes;
}
