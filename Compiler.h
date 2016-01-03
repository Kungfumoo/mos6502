//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Class responsible for compiling the code.
 */

#include <string>

#ifndef _COMPILER_H_
#define _COMPILER_H_
namespace MOS_6502
{
    class Compiler
    {
        //--General(private)
        void compileLine(std::string line); //Compiles a line of code and adds it to memory and any operands to the stack

    public:
        //--General
        void compile(std::string src);
        void compileFromFile(std::string fileName);

        //Constructor
        Compiler();
    };
}
#endif
