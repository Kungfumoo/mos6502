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
		enum AddressingModes
		{
			IMMEDIATE,
			ABSOLUTE,
			ZEROPAGE,
			IMPLIED,
			ACCUMULATOR,
			INDEXED_X,
			INDEXED_Y,
			ZEROPAGE_INDEXED,
			INDIRECT,
			PRE_INDEXED_INDIRECT,
			POST_INDEXED_INDIRECT,
			RELATIVE
		};

		struct CompilerState
		{
			unsigned int lineNo;
		};

		//Constants
		static const byte ADDRESSING_MODES;
		static const byte INVALID_MODE;

		//Variables
        std::unordered_map<char, std::vector<std::string>> _commands;
		std::unordered_map<std::string, std::vector<byte>> _oppcodes;
		CompilerState _state;

        //--General(private)
        std::string fetchCommand(std::string& line); //fetch the command from the line, chucks an exception if none found
		std::vector<byte> fetchOppcodes(std::string& command, std::string& line); //converts the line into oppcodes
		std::string stripComments(std::string& line); //strips comments from a line of code
        std::vector<byte> compileLine(std::string& line); //Compiles a line of code and adds it to memory and any operands to the stack
		
		void resetState();
        void setupCommands();
		void setupOppcodes();

    public:
        //--General
		std::vector<byte> compile(std::string src);
		std::vector<byte> compileFromFile(std::string fileName); //NOTE: Heap or stack, depends on how big the programs will be

        //Constructor
        Compiler();
    };
}
#endif
