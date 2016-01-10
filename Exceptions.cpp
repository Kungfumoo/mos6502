//Houses implementation for all the exception classes
#include "Exceptions.h"
#include <string>

using namespace MOS_6502;
using namespace std;

//--UnknownOpCodeException
const char* UnknownOpCodeException::what() const noexcept
{
	return _error.c_str();
}

UnknownOpCodeException::UnknownOpCodeException(unsigned int opcode)
{
	_error = "Unknown OpCode Given(base 10): " + to_string(opcode);
}

//--BitLimitExceededException
const char* BitLimitExceededException::what() const noexcept
{
    return "Exceeded number of bits that this system is capable of storing! (8 bits)";
}

//--CompilerException
const char* CompilerException::what() const noexcept
{
	return _error.c_str();
}

CompilerException::CompilerException(unsigned int lineNo, string& message)
{
	_error = "COMPILER ERROR (ln: " + to_string(lineNo) + "): " + message;
}
