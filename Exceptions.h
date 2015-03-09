//Author: Aaron Winter
//Date: 10/9/2014
/*Description:
 *Header containing exception declarations for use in this emulator.
 */

#include <exception>

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
#ifdef _WIN32
namespace MOS_6502
{
    class UnknownOpCodeException : public std::exception
    {
        //Variables
        unsigned int _opcode;

    public:
        //--Overriden Methods:
        const char* what() const override;

        //--Constructor
        UnknownOpCodeException(unsigned int opcode);
    };

    class BitLimitExceededException : public std::exception
    {
    public:
        //--Overriden Methods:
        const char* what() const override;
    };
}
#else
namespace MOS_6502
{
    class UnknownOpCodeException : public std::exception
    {
        //Variables
        unsigned int _opcode;

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
}
#endif
#endif
