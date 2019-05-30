#include <iostream>
#include <exception>
#include "MOS6502/Test/CpuTest.h"

using namespace std;
using namespace MOS_6502::Test;

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
