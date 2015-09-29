//Compile class implementation
#include "Compiler.h"
#include "MOS6502CPU.h"
#include <iostream>
#include <fstream>

using namespace MOS_6502;
using namespace std;

//--General(private)
void Compiler::compileLine(string line)
{

}

//--General(public)
void Compiler::compile(string src)
{
    cout << src << endl; //TODO: TEMP
}

void Compiler::compileFromFile(string filePath)
{
    string src = "";
    ifstream io;

    io.open(filePath);

    if(io.fail())
    {
        //TODO: throw exception
    }

    //TODO: open file
    //TODO: read all contents into a string

    io.close();
    compile(src);
}

//--Constructor
Compiler::Compiler(MOS6502CPU* cpuRef)
{
    _cpu = cpuRef;
}
