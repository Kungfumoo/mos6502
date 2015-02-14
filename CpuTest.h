//Author: Aaron Winter
//Date: 14/9/2014
/*Description:
 *Class responsible for testing the MOS6502CPU class, by performing operations and checking
 *state before and after. This follows the unit test method.
 *
 *To ensure accuracy of tests, a new memory and cpu object is built for every test case.
 */

#ifndef _CPU_TEST_H_
#define _CPU_TEST_H_
namespace MOS_6502
{
    class CpuTest
    {
        //Constants(private)
        static const unsigned int MEMORY_SIZE;

    public:
        //--Test Cases:
        bool testADC1();
        bool testADC3();
        bool testADC7();
        bool testADC2();
        bool testADC6_X();
        bool testADC6_Y();
        bool testADC9();
        bool testADC10();

        bool testAND(); //just one, as it uses the same addressing modes as ADC

        bool testASL5();
        bool testASL3();

        bool testBCC();
        bool testBCS();
        bool testBEQ();
        bool testBMI();
        bool testBNE();
        bool testBPL();
        bool testBVC();
        bool testBVS();

        bool testBIT();

        bool testClears();

        bool testBRK();

        bool testCMP(); //just one, addressing modes are already being tested.
        bool testCPY();
        bool testCPX();

        bool testDEC();
        bool testDEY();
        bool testDEX();

        bool testLDA1();

        void runTests(); //runs all tests
    };
}
#endif
