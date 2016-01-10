//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Header containing exception declarations for use in this emulator.
 */

#include <exception>
#include <string>

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
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

	public:
		//--Overriden Methods:
		const char* what() const noexcept override;

		//--Constructor
		CompilerException(unsigned int line, std::string& message);
	};
}
#endif
