//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Class responsible for compiling the code. Is a friend of MOS6502CPU and can access its
 *privates directly
 */

#include <string>

#ifndef _COMPILER_H_
#define _COMPILER_H_
namespace MOS_6502
{
    //Forward decs
    class MOS6502CPU;

    class Compiler
    {
        //Variables
        MOS6502CPU* _cpu; //Reference to the CPU using this compiler DO NOT DESTROY

        //--General(private)
        void compileLine(std::string line); //Compiles a line of code and adds it to memory and any operands to the stack

    public:
        //--General
        void compile(std::string src);
        void compileFromFile(std::string fileName);

        //Constructor
        Compiler(MOS6502CPU* cpuRef);
    };
}
#endif
