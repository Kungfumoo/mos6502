//Compile class implementation
#include "Compiler.h"
#include <iostream>
#include <fstream>
#include <regex>

using namespace MOS_6502;
using namespace std;

//--General(private)
string Compiler::fetchCommand(string& line)
{
    //TODO: fetch first letter, use letter to access bucket, iterate through bucket and return the command. Exception if none found
    return "";
}

string Compiler::stripComments(string& line)
{
	unsigned int i;
	bool found = false;

	//TODO: remove whitespace?

	for(i = 0; i < line.length(); i++)
	{
		if(line[i] == ';')
		{
			found = true;
			break;
		}
	}

	if(found)
		return line.substr(0, i);

	return line;
}

vector<byte> Compiler::compileLine(string& line)
{
	cout << line << endl; //TODO: TEMP

	vector<byte> oppcodes;

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

	//test regex matches
	if(regex_search(line, regex("LDA")))
	{
		cout << "match" << endl;
	}

	return oppcodes;
}

void Compiler::setupCommands()
{
    vector<string>::iterator i;

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
        //TODO: throw exception
		cout << "error" << endl;
		return vector<byte>();
    }

	vector<byte> program;

	if(io.is_open())
	{
		while(io.good())
		{
			string line;

			//read and compile each line
			getline(io, line);

			vector<byte> oppcodes = compileLine(line);

			for(vector<byte>::iterator i = oppcodes.begin(); i < oppcodes.end(); i++)
			{
				program.insert(program.end(), *i);
			}
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
