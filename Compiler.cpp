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
    //fetch first letter, use letter to access bucket, iterate through bucket and return the command. Exception if none found
    //This assumes the line has been trimmed
    auto i = _commands->find(line[0]);

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
    vector<byte> oppCodes;

    //work out addressing mode
    try {
        if(regex_search(line, regex("^[A-Z]{3}\ #[\$]?[0-9A-Za-z]{2}$"))) //1 immediate
        {
            string operand = line.substr(5);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::IMMEDIATE));

            if(operand[0] == '$') //literal hex
                oppCodes.push_back((byte)(stoi(operand.substr(1), nullptr, 16)));
            else //literal decimal
                oppCodes.push_back((byte)(stoi(operand, nullptr, 10)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \$[0-9A-Za-z]{4}$"))) //2 absolute
        {
            unsigned short operand = (unsigned short)stoi(line.substr(5), nullptr, 16);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ABSOLUTE));
            oppCodes.push_back((byte)((operand >> 8) & 255));
            oppCodes.push_back((byte)operand & 255);
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \$[0-9A-Za-z]{2}$"))) //3 zeropage
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ZEROPAGE));
            oppCodes.push_back((byte)(stoi(line.substr(5), nullptr, 16)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}$"))) //4 implied
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::IMPLIED));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ A$"))) //5 accumulator
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ACCUMULATOR));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \$[0-9A-Za-z]{4},[X|Y]$"))) //6 absolute indexed
        {
            OppCodeMap::AddressingModes mode = (line.back() == 'X') ? OppCodeMap::AddressingModes::INDEXED_X : OppCodeMap::AddressingModes::INDEXED_Y;
            unsigned short operand = (unsigned short)stoi(line.substr(5, 4), nullptr, 16);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, mode));
            oppCodes.push_back((byte)((operand >> 8) & 255));
            oppCodes.push_back((byte)operand & 255);
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \$[0-9A-Za-z]{2},[X|Y]$"))) //7 zeropage indexed
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ZEROPAGE_INDEXED));
            oppCodes.push_back((byte)(stoi(line.substr(5, 2), nullptr, 16)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \(\$[0-9A-Za-z]{4}\)$"))) //8 Indirect
        {
            unsigned short operand = (unsigned short)stoi(line.substr(6, 4), nullptr, 16);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::INDIRECT));
            oppCodes.push_back((byte)((operand >> 8) & 255));
            oppCodes.push_back((byte)operand & 255);
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \(\$[0-9A-Za-z]{2},X\)$"))) //9 pre-indexed indirect
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::PRE_INDEXED_INDIRECT));
            oppCodes.push_back((byte)(stoi(line.substr(6, 2), nullptr, 16)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \(\$[0-9A-Za-z]{2}\),Y$"))) //10 post-indexed indirect
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::POST_INDEXED_INDIRECT));
            oppCodes.push_back((byte)(stoi(line.substr(6, 2), nullptr, 16)));
        }
        else
        {
            throw new CompilerException("Syntax Error");
        }
    }
    catch(CompilerException* e)
    {
        e->appendLine(_state.lineNo);
        throw e;
    }


	return oppCodes;
}

string Compiler::stripAndTrim(string& line)
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

    //uppercase
    for(auto i = line.begin(); i < line.end(); i++)
        *i = toupper(*i);

	return line;
}

vector<byte> Compiler::compileLine(string& line)
{
	if(line.empty())
        return vector<byte>();

	//strip comments from line
	line = stripAndTrim(line);

    string command = fetchCommand(line);
    vector<byte> oppcodes = fetchOppcodes(command, line);

    return oppcodes;
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
