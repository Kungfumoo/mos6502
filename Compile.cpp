//Compile class implementation
#include "Compiler.h"
#include "MOS6502CPU.h"

using namespace MOS_6502;
using namespace std;

//--General(private)
void Compiler::compileLine(string line)
{

}

//--General(public)
void Compiler::compile(string src)
{

}

//--Constructor
Compiler::Compiler(MOS6502CPU* cpuRef)
{
    _cpu = cpuRef;
}
