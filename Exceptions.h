//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Header containing exception declarations for use in this emulator.
 */

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>
#include <string>

namespace MOS_6502
{
    class UnknownOpCodeException : public std::exception
    {
        //Variables
		std::string _error;

    public:
        //--Overriden Methods:
        const char* what() const noexcept override;

        //--Constructor
        UnknownOpCodeException(unsigned int opcode);
    };

    class BitLimitExceededException : public std::exception
    {
    public:
        //--Overriden Methods:
        const char* what() const noexcept override;
    };

	class CompilerException : public std::exception
	{
		//Variables
		std::string _error;

		//--Private Methods:
		void composeMessage(unsigned int& line, std::string& message);

	public:
		//--Overriden Methods:
		const char* what() const noexcept override;

		//--Public Methods:
		void appendLine(unsigned int line);

		//--Constructor
		CompilerException(unsigned int line, std::string message);
		CompilerException(std::string error);
	};
}
#endif
