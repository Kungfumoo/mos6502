//Compile class implementation
#include "Compiler.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "Exceptions.h"
#include "CommandMap.h"
#include "OppCodeMap.h"
#include "MOS6502CPU.h"

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
        /*
         * TODO: Had to double escape the $, bug in C++ regex engine?
         * Consider using boost instead?
         */

        if(regex_search(line, regex("^JMP\ [A-Za-z]+$"))) //JMP Absolute with label
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ABSOLUTE));

            string label = line.substr(4);

            auto i = _state.labels.find(label);

            if(i != _state.labels.end())
            {
                unsigned short addr = (unsigned short)(MOS6502CPU::DEFAULT_PC + i->second + 1);

                oppCodes.push_back((byte)addr & 255);
                oppCodes.push_back((byte)((addr >> 8) & 255));
            }
            else //add it to labels to update
            {
                auto n = _state.labelsToUpdate.find("label");

                if(n == _state.labelsToUpdate.end())
                    _state.labelsToUpdate[label] = vector<unsigned int>();

                vector<unsigned int>* addresses = &_state.labelsToUpdate[label];
                addresses->push_back(_state.index + 1);

                //add placeholder
                oppCodes.push_back(0);
                oppCodes.push_back(0);
            }
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ #[\$]?[0-9A-Za-z]{2}$"))) //1 immediate
        {
            string operand = line.substr(5);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::IMMEDIATE));

            if(operand[0] == '$') //literal hex
                oppCodes.push_back((byte)(stoi(operand.substr(1), nullptr, 16)));
            else //literal decimal
                oppCodes.push_back((byte)(stoi(operand, nullptr, 10)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \\$[0-9A-Za-z]{4}$"))) //2 absolute
        {
            unsigned short operand = (unsigned short)stoi(line.substr(5), nullptr, 16);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ABSOLUTE));
            oppCodes.push_back((byte)operand & 255);
            oppCodes.push_back((byte)((operand >> 8) & 255));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \\$[0-9A-Za-z]{2}$"))) //3 zeropage
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
        else if(regex_search(line, regex("^[A-Z]{3}\ \\$[0-9A-Za-z]{4},[X|Y]$"))) //6 absolute indexed
        {
            OppCodeMap::AddressingModes mode = (line.back() == 'X') ? OppCodeMap::AddressingModes::INDEXED_X : OppCodeMap::AddressingModes::INDEXED_Y;
            unsigned short operand = (unsigned short)stoi(line.substr(5, 4), nullptr, 16);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, mode));
            oppCodes.push_back((byte)operand & 255);
            oppCodes.push_back((byte)((operand >> 8) & 255));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \\$[0-9A-Za-z]{2},[X|Y]$"))) //7 zeropage indexed
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::ZEROPAGE_INDEXED));
            oppCodes.push_back((byte)(stoi(line.substr(5, 2), nullptr, 16)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \(\\$[0-9A-Za-z]{4}\)$"))) //8 Indirect
        {
            unsigned short operand = (unsigned short)stoi(line.substr(6, 4), nullptr, 16);

            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::INDIRECT));
            oppCodes.push_back((byte)operand & 255);
            oppCodes.push_back((byte)((operand >> 8) & 255));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \(\\$[0-9A-Za-z]{2},X\)$"))) //9 pre-indexed indirect
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::PRE_INDEXED_INDIRECT));
            oppCodes.push_back((byte)(stoi(line.substr(6, 2), nullptr, 16)));
        }
        else if(regex_search(line, regex("^[A-Z]{3}\ \(\\$[0-9A-Za-z]{2}\),Y$"))) //10 post-indexed indirect
        {
            oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::POST_INDEXED_INDIRECT));
            oppCodes.push_back((byte)(stoi(line.substr(6, 2), nullptr, 16)));
        }
		else if(regex_search(line, regex("^[A-Z]{3}\ (\\$[0-9A-Za-z]{2}|[A-Za-z]+)$"))) //11 Relative
		{
			oppCodes.push_back(_oppcodes->fetchCommandCode(command, OppCodeMap::AddressingModes::RELATIVE));

			if(line[4] == '$') //numerical
			{
				oppCodes.push_back((byte)(stoi(line.substr(5), nullptr, 16)));
			}
			else //using labels
			{
				string label = line.substr(4);

				auto i = _state.labels.find(label);

				if(i != _state.labels.end())
				{
					unsigned int value = 128 + (_state.index - i->second); //signed maginitude

					if(value > 255)
						throw new CompilerException(_state.lineNo, "A negative Branch instruction exceeds a branch greater then 255, consider using a jmp");

					oppCodes.push_back((byte)value);
				}
				else //add it to labels to update
				{
					auto n = _state.labelsToUpdate.find("label");

					if(n == _state.labelsToUpdate.end())
						_state.labelsToUpdate[label] = vector<unsigned int>();

					vector<unsigned int>* addresses = &_state.labelsToUpdate[label];
					addresses->push_back(_state.index + 1);

					oppCodes.push_back(0); //add placeholder
				}
			}
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

	if(!checkForLabel(line))
	{
		string command = fetchCommand(line);
		vector<byte> oppcodes = fetchOppcodes(command, line);

		return oppcodes;
	}

	return vector<byte>();
}

bool Compiler::checkForLabel(string& line)
{
	if(regex_search(line, regex("^[A-Za-z]+:$"))) //TODO: clarify if numbers can be used in labels
	{
		string sub = line.substr(0, line.length() - 1);
		unsigned int labelPos = _state.index - 1; //index starts from 1 so -1 is needed

		//Update compiler state with label
		try
		{
			_state.labels.insert(pair<string, unsigned int>(sub, labelPos));
		}
		catch(exception* e)
		{
			throw new CompilerException(_state.lineNo, "label: \"" + sub + "\" already defined!");
		}

		//Update any commands awaiting the position of this label (will always be positive)
		auto i = _state.labelsToUpdate.find(sub);

		if(i != _state.labelsToUpdate.end() && !i->second.empty())
		{
			vector<unsigned int>* indexes = &i->second;

			for(auto n = indexes->begin(); n < indexes->end(); n++)
			{
			    if(_state.program[(*n) - 1] == 0x4C) //then it's a JMP, record location of label, not diff
                {
                    labelPos = MOS6502CPU::DEFAULT_PC + labelPos + 1;
                    _state.program[*n] = (byte)labelPos & 255;
                    _state.program[(*n) + 1] = (byte)((labelPos >> 8) & 255);
                }
                else
                {
                    unsigned int diff = labelPos - *n;

                    if(diff > 127)
                        throw new CompilerException(*n, "A positive Branch instruction exceeds a branch greater then 127, consider using a jmp");

                    _state.program[*n] = diff; //branch instructions use signed magnitude
                }
			}

			indexes->clear();
		}


		return true;
	}

	return false;
}

void Compiler::resetState()
{
	_state.lineNo = 1;
	_state.index = 0;
	_state.program.clear();
	_state.labels.clear();
	_state.labelsToUpdate.clear();
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
					_state.program.insert(_state.program.end(), *i);
					_state.index++;
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
	return _state.program;
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
