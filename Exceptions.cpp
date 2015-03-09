//Houses implementation for all the exception classes
#include "Exceptions.h"
#include <string>

using namespace MOS_6502;
using namespace std;

#ifdef _WIN32
//--UnknownOpCodeException
const char* UnknownOpCodeException::what() const
{
    string error = "Unknown OpCode Given(base 10): " + to_string(_opcode);

    return error.c_str();
}

UnknownOpCodeException::UnknownOpCodeException(unsigned int opcode)
{
    _opcode = opcode;
}

//--BitLimitExceededException
const char* BitLimitExceededException::what() const
{
    return "Exceeded number of bits that this system is capable of storing! (8 bits)";
}
#else
//--UnknownOpCodeException
const char* UnknownOpCodeException::what() const noexcept
{
    string error = "Unknown OpCode Given(base 10): " + to_string(_opcode);

    return error.c_str();
}

UnknownOpCodeException::UnknownOpCodeException(unsigned int opcode)
{
    _opcode = opcode;
}

//--BitLimitExceededException
const char* BitLimitExceededException::what() const noexcept
{
    return "Exceeded number of bits that this system is capable of storing! (8 bits)";
}
#endif
