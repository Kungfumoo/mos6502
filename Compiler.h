//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Class responsible for compiling the code.
 */

#include <string>
#include <vector>

typedef unsigned char byte;

#ifndef _COMPILER_H_
#define _COMPILER_H_
namespace MOS_6502
{
    class Compiler
    {
        //--General(private)
        std::vector<byte> compileLine(std::string line); //Compiles a line of code and adds it to memory and any operands to the stack

    public:
        //--General
		std::vector<byte> compile(std::string src);
		std::vector<byte> compileFromFile(std::string fileName); //Heap or stack, depends on how big the programs will be

        //Constructor
        Compiler();
    };
}
#endif
