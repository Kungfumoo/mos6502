//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Class responsible for compiling the code.
 */

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>
#include <vector>
#include <unordered_map>

typedef unsigned char byte;

namespace MOS_6502
{
    namespace CompilerAssets
    {
        //forward dec
        class CommandMap;
		class OppCodeMap;
    }

    class Compiler
    {
		struct CompilerState
		{
			unsigned int lineNo;
			unsigned int index;
			std::unordered_map<std::string, unsigned int> labels;
			std::unordered_map<std::string, std::vector<unsigned int>> labelsToUpdate; //key=label, value = array of indexes to update with label location
			std::vector<byte> program;
		};

		//Variables
		CompilerAssets::CommandMap* _commands;
		CompilerAssets::OppCodeMap* _oppcodes;
		CompilerState _state;

        //--General(private)
        std::string fetchCommand(std::string& line); //fetch the command from the line, chucks an exception if none found
		std::vector<byte> fetchOppcodes(std::string& command, std::string& line); //converts the line into oppcodes
		std::string stripAndTrim(std::string& line); //strips comments, whitespace from a line of code also uppercases
        std::vector<byte> compileLine(std::string& line); //Compiles a line of code and adds it to memory and any operands to the stack
		bool checkForLabel(std::string& line); //Checks for a label, if one exists it will update the compiler state and return true

		void resetState();

    public:
        //--General
		std::vector<byte> compile(std::string src);
		std::vector<byte> compileFromFile(std::string fileName); //NOTE: Heap or stack, depends on how big the programs will be

        //Constructor
        Compiler();

        //Destructor
        ~Compiler();
    };
}
#endif
