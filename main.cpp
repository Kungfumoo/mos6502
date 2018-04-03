#include <iostream>
#include <exception>
#include "MOS6502CPU.h"
#include "BasicMemory.h"
#include "CpuTest.h"
#include "Compiler.h"

using namespace std;
using namespace MOS_6502;

int main()
{
    //TESTS:
    CpuTest* testSuite = new CpuTest();
    testSuite->runTests();

    delete testSuite;

#ifdef _WIN32
	system("pause");
#endif

    return 0;
}
