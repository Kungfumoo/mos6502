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
void CompilerException::composeMessage(unsigned int& line, string& message)
{
    _error = "COMPILER ERROR (ln: " + to_string(line) + "): " + message;
}

const char* CompilerException::what() const noexcept
{
	return _error.c_str();
}

void CompilerException::appendLine(unsigned int line)
{
    composeMessage(line, _error);
}

CompilerException::CompilerException(unsigned int line, string message)
{
	composeMessage(line, message);
}

CompilerException::CompilerException(string error)
{
    _error = error;
}
