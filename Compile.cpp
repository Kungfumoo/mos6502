//Compile class implementation
#include "Compiler.h"
#include <iostream>
#include <fstream>

using namespace MOS_6502;
using namespace std;

//--General(private)
vector<byte> Compiler::compileLine(string line)
{
	cout << line << endl; //TODO: TEMP
	return vector<byte>();
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
{}
