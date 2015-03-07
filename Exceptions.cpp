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
#endif
