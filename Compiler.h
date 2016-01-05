//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Class responsible for compiling the code.
 */

#include <string>
#include <vector>
#include <unordered_map>

typedef unsigned char byte;

#ifndef _COMPILER_H_
#define _COMPILER_H_
namespace MOS_6502
{
    class Compiler
    {
        std::unordered_map<char, std::vector<std::string>> _commands;

        //--General(private)
        std::string fetchCommand(std::string& line); //fetch the command from the line, chucks an exception if none found
		std::string stripComments(std::string& line); //strips comments from a line of code
        std::vector<byte> compileLine(std::string& line); //Compiles a line of code and adds it to memory and any operands to the stack

        void setupCommands();

    public:
        //--General
		std::vector<byte> compile(std::string src);
		std::vector<byte> compileFromFile(std::string fileName); //Heap or stack, depends on how big the programs will be

        //Constructor
        Compiler();
    };
}
#endif
