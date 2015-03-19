//CpuTest class implementation
#include "CpuTest.h"
#include "MOS6502CPU.h"
#include "BasicMemory.h"
#include "Stack.h"
#include "StatusRegister.h"
#include "Utility.h"
#include <iostream>

using namespace MOS_6502;
using namespace std;

//const definitions
const unsigned int CpuTest::MEMORY_SIZE = 65536;

//--Test Cases
bool CpuTest::testADC1()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     *6 - BCD addition
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;

    //setup memory
    memory->write(0x69, counter++); //write ADC instruction to next byte
    memory->write(0x02, counter++); //ADC operand

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC1(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;

    //setup memory
    memory->write(0x69, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++); //ADC operand

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC1(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;

    //setup memory
    memory->write(0x69, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++); //ADC operand

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC1(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x69, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC1(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x69, counter++); //write ADC instruction to next byte
    memory->write(0x01, counter++); //ADC operand

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC1(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 6
    /*Operation: 15(0001 0101 | 0x15) + 31(0011 0001 | 0x31)
     *Expected: 0x46, Status: others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setD(true);
    cpu->_accumulator = 0x15;

    //setup memory
    memory->write(0x69, counter++); //write ADC instruction to next byte
    memory->write(0x31, counter++); //ADC operand

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x46 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC1(): test case 6 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC1(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC3()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;

    //setup memory
    memory->write(0x65, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x02, 0x0A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC3(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;

    //setup memory
    memory->write(0x65, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x05, 0x0A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC3(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;

    //setup memory
    memory->write(0x65, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x00, 0x0A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC3(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x65, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x0A, 0x0A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC3(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x65, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x01, 0x0A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC3(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC3(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC7()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;
    cpu->_x = 0x01;

    //setup memory
    memory->write(0x75, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x02, 0x0A + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC7(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;
    cpu->_x = 0x01;

    //setup memory
    memory->write(0x75, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x05, 0x0A + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC7(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;
    cpu->_x = 0x01;

    //setup memory
    memory->write(0x75, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x00, 0x0A + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC7(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_x = 0x01;

    //setup memory
    memory->write(0x75, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x0A, 0x0A + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC7(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers00FF
    cpu->_accumulator = 0xFF;
    cpu->_x = 0x01;

    //setup memory
    memory->write(0x75, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x01, 0x0A + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC7(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC7(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC2()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;

    //setup memory
    memory->write(0x6D, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x02, 0x0800);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC2(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;

    //setup memory
    memory->write(0x6D, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x05, 0x0800);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC2(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;

    //setup memory
    memory->write(0x6D, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x00, 0x0800);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC2(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x6D, counter++); //write ADC instruction to next byte
    memory->write(0x6D, counter++);
    memory->write(0x1F, counter++);
    memory->write(0x0A, 0x1F6D);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC2(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers00FF
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x6D, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x2B, counter++);
    memory->write(0x01, 0x2B0A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC2(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC2(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC6_X()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0x7D, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x02, 0x0800 + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC6_X(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0x7D, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x05, 0x0800 + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC6_X(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;
    cpu->_x = 0xAA;

    //setup memory
    memory->write(0x7D, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x00, 0x0800 + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC6_X(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_x = 0xAA;

    //setup memory
    memory->write(0x7D, counter++); //write ADC instruction to next byte
    memory->write(0x6D, counter++);
    memory->write(0x1F, counter++);
    memory->write(0x0A, 0x1F6D + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC6_X(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers00FF
    cpu->_accumulator = 0xFF;
    cpu->_x = 0x01;

    //setup memory
    memory->write(0x7D, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x2B, counter++);
    memory->write(0x01, 0x2B0A + cpu->_x);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC6_X(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC6_X(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC6_Y()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0x79, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x02, 0x0800 + cpu->_y);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC6_Y(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0x79, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x05, 0x0800 + cpu->_y);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC6_Y(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;
    cpu->_y = 0xAA;

    //setup memory
    memory->write(0x79, counter++); //write ADC instruction to next byte
    memory->write(0x00, counter++);
    memory->write(0x08, counter++);
    memory->write(0x00, 0x0800 + cpu->_y);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC6_Y(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_y = 0xAA;

    //setup memory
    memory->write(0x79, counter++); //write ADC instruction to next byte
    memory->write(0x6D, counter++);
    memory->write(0x1F, counter++);
    memory->write(0x0A, 0x1F6D + cpu->_y);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC6_Y(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers00FF
    cpu->_accumulator = 0xFF;
    cpu->_y = 0x01;

    //setup memory
    memory->write(0x79, counter++); //write ADC instruction to next byte
    memory->write(0x0A, counter++); //ADC operand
    memory->write(0x2B, counter++);
    memory->write(0x01, 0x2B0A + cpu->_y);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC6_Y(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC6_Y(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC9()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0x61, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++);
    memory->write(0x00, 0x0F);
    memory->write(0x08, 0x10);
    memory->write(0x02, 0x0800);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC9(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0x61, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++);
    memory->write(0x00, 0x0F);
    memory->write(0x08, 0x10);
    memory->write(0x05, 0x0800);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC9(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;
    cpu->_x = 0xAA;

    //setup memory
    memory->write(0x61, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++);
    memory->write(0x00, 0xAF);
    memory->write(0x08, 0xB0);
    memory->write(0x00, 0x0800);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC9(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_x = 0xAA;

    //setup memory
    memory->write(0x61, counter++); //write ADC instruction to next byte
    memory->write(0x2B, counter++);
    memory->write(0x6D, 0xD5);
    memory->write(0x1F, 0xD6);
    memory->write(0x0A, 0x1F6D);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC9(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_x = 0xAA;

    //setup memory
    memory->write(0x61, counter++); //write ADC instruction to next byte
    memory->write(0x2B, counter++);
    memory->write(0x6D, 0xD5);
    memory->write(0x1F, 0xD6);
    memory->write(0x01, 0x1F6D);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC9(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC9(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC10()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test adding two positive numbers together to yield a positive result
     *2 - Test adding a number to 127 that yields a negative result(S flag)
     *3 - Test adding a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test adding numbers together that yield higher then 255.
     *5 - An overflow that results in zero
     */

    //Test 1
    /*Operation: 0x01 + 0x02
     *Expected result: 0x03, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0x71, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++);
    memory->write(0x00, 0x05);
    memory->write(0x08, 0x06);
    memory->write(0x02, 0x080A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x03 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC10(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 127 + 5
     *Expected: Signed 134, Status: S = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x7F;
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0x71, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++);
    memory->write(0x00, 0x05);
    memory->write(0x08, 0x06);
    memory->write(0x05, 0x080A);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC10(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0 + 0
     *Expected: 0, Status: Z = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;
    cpu->_y = 0xAA;

    //setup memory
    memory->write(0x71, counter++); //write ADC instruction to next byte
    memory->write(0x05, counter++);
    memory->write(0x00, 0x05);
    memory->write(0x08, 0x06);
    memory->write(0x00, 0x08AA);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testADC10(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 255 + 10
     *Expected: 9, Status: V = 1, C = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_y = 0xAA;

    //setup memory
    memory->write(0x71, counter++); //write ADC instruction to next byte
    memory->write(0x2B, counter++);
    memory->write(0x6D, 0x2B);
    memory->write(0x1F, 0x2C);
    memory->write(0x0A, 0x2017);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC10(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 5
    /*Operation: 255 + 1
     *Expected: 0, Status: Z = 1, C = 1, V = 1, others = false.
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;
    cpu->_y = 0xAA;

    //setup memory
    memory->write(0x71, counter++); //write ADC instruction to next byte
    memory->write(0x2B, counter++);
    memory->write(0x6D, 0x2B);
    memory->write(0x1F, 0x2C);
    memory->write(0x01, 0x2017);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == true))
    {
        cout << "testADC10(): test case 5 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testADC10(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testAND()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test ANDing two positive numbers together to yield a positive result
     *2 - Test ANDing a number that yields a negative result(S flag)
     *3 - Test ANDing a 0 + 0 to yield a 0 result(Z flag)
     */

    //Test 1
    /*Operation: 0x9B /\ 0x1F
     *Expected result: 0x1B, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x9B;

    //setup memory
    memory->write(0x29, counter++); //AND
    memory->write(0x1F, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x1B &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testAND(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 0xFF /\ 0xA0
     *Expected result: 0xA0, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x29, counter++); //AND
    memory->write(0xA0, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0xA0 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testAND(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0x9B /\ 0x00
     *Expected result: 0x00, Z = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x9B;

    //setup memory
    memory->write(0x29, counter++); //AND
    memory->write(0x00, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testAND(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testAND(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testASL5()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Shifting a number to yield a positive result
     *2 - Shifting a number that yields a negative result(S flag)
     *3 - Shifting a number to yield a 0 result(Z flag)
     *4 - Shifting a number to yield a carry
     */

    //Test 1
    /*Operation: 0x0A << 1
     *Expected result: 0x14, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x0A;

    //setup memory
    memory->write(0x0A, counter++); //ASL

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x14 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testASL5(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 0x40 << 1
     *Expected result: 0x80, S = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x40;

    //setup memory
    memory->write(0x0A, counter++); //ASL

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x80 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testASL5(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0x80 << 1
     *Expected result: 0x00, C = true, Z = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x80;

    //setup memory
    memory->write(0x0A, counter++); //ASL

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testASL5(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 0xC0 << 1
     *Expected result: 0x80, C = true, S = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xC0;

    //setup memory
    memory->write(0x0A, counter++); //ASL

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x80 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testASL5(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testASL5(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testASL3()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Shifting a number to yield a positive result
     *2 - Shifting a number that yields a negative result(S flag)
     *3 - Shifting a number to yield a 0 result(Z flag)
     *4 - Shifting a number to yield a carry
     */

    //Test 1
    /*Operation: 0x0A << 1
     *Expected result: 0x14, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //other:
    byte operandAddress = 0xBC;

    //setup memory
    memory->write(0x06, counter++); //ASL
    memory->write(operandAddress, counter++);
    memory->write(0x0A, operandAddress);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(operandAddress) == 0x14 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testASL3(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");
        cout << "\nValue at[0x" << std::hex << (int)operandAddress << "] = 0x" << (int)memory->read(operandAddress) << endl;

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 0x40 << 1
     *Expected result: 0x80, S = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //other:
    operandAddress = 0xBC;

    //setup memory
    memory->write(0x06, counter++); //ASL
    memory->write(operandAddress, counter++);
    memory->write(0x40, operandAddress);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(operandAddress) == 0x80 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testASL3(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");
        cout << "\nValue at[0x" << std::hex << (int)operandAddress << "] = 0x" << (int)memory->read(operandAddress) << endl;

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0x80 << 1
     *Expected result: 0x00, C = true, Z = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //other:
    operandAddress = 0x52;

    //setup memory
    memory->write(0x06, counter++); //ASL
    memory->write(operandAddress, counter++);
    memory->write(0x80, operandAddress);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(operandAddress) == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testASL3(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");
        cout << "\nValue at[0x" << std::hex << (int)operandAddress << "] = 0x" << (int)memory->read(operandAddress) << endl;

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 0xC0 << 1
     *Expected result: 0x80, C = true, S = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //other:
    operandAddress = 0x53;

    //setup memory
    memory->write(0x06, counter++); //ASL
    memory->write(operandAddress, counter++);
    memory->write(0xC0, operandAddress);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(operandAddress) == 0x80 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testASL3(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");
        cout << "\nValue at[0x" << std::hex << (int)operandAddress << "] = 0x" << (int)memory->read(operandAddress) << endl;

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testASL3(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBCC()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x90, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBCC(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, All status bits false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup memory
    memory->write(0x90, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBCC(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110) with carry true
     *Expected result: 0x602, C = 1, others = 0
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setC(true);

    //setup memory
    memory->write(0x90, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testBCC(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBCC(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBCS()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, C = 1, others = 0
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setC(true);

    //setup memory
    memory->write(0xB0, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testBCS(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, C = 1, others = 0
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setC(true);

    //setup memory
    memory->write(0xB0, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testBCS(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110) with carry false
     *Expected result: 0x602, all status bits false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xB0, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBCS(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBCS(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBEQ()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, Z = 1, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setZ(true);

    //setup memory
    memory->write(0xF0, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBEQ(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, Z = 1, others = 0
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setZ(true);

    //setup memory
    memory->write(0xF0, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBEQ(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110) with carry true
     *Expected result: 0x602, all status bits false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xF0, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBEQ(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBEQ(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBMI()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, S = 1, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setS(true);

    //setup memory
    memory->write(0x30, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBMI(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, S = 1, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setS(true);

    //setup memory
    memory->write(0x30, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBMI(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x602, all status bits false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x30, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBMI(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBMI(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBNE()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xD0, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBNE(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xD0, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBNE(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x602, Z = 1, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setZ(true);

    //setup memory
    memory->write(0xD0, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBNE(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBNE(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBPL()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x10, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBPL(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x10, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBPL(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x602, S = 1, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setS(true);

    //setup memory
    memory->write(0x10, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBPL(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBPL(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBVC()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x50, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBVC(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x50, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBVC(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x602, S = 1, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setV(true);

    //setup memory
    memory->write(0x50, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == true))
    {
        cout << "testBVC(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBVC(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBVS()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Branch with Negative number
     *2 - Branch with Positive number
     *3 - Branch with branch condition as false
     */

    //Test 1
    /*Operation: Branch to 0xA7 (-39)
     *Expected result: 0x5DC, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setV(true);

    //setup memory
    memory->write(0x70, counter++); //operation
    memory->write(0xA7, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x5DB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == true))
    {
        cout << "testBVS(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x670, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setV(true);

    //setup memory
    memory->write(0x70, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x670 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == true))
    {
        cout << "testBVS(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: Branch to 0x6E (+110)
     *Expected result: 0x602, S = 1, others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x70, counter++); //operation
    memory->write(0x6E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x602 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBVS(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBVS(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBIT()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test bit with A = 42, O = 127
     *2 - Test bit with A = 32, O = 73
     */

    //Test 1
    /*Operation: Bit test with A = 42, O = 127
     *Expected result: V = 1, Others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 42;

    //setup memory
    memory->write(0x24, counter++); //operation(BIT3)
    memory->write(0xC4, counter++);
    memory->write(0x7F, 0xC4);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == true))
    {
        cout << "testBIT(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: Bit test with A = 32, O = 73
     *Expected result: V = 1, Z = 1, Others = false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 32;

    //setup memory
    memory->write(0x2C, counter++); //operation(BIT2)
    memory->write(0x10, counter++);
    memory->write(0xA1, counter++);
    memory->write(0x49, 0xA110);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == true))
    {
        cout << "testBIT(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testBIT(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testClears()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test CLC4 - carry clear
     *2 - Test CLD4 - decimal clear
     *3 - Test CLI4 - interupt clear
     *4 - Test CLV4 - overflow clear
     */

    //Test 1
    /*Operation: CLC4 - carry clear
     *Expected result: all false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setC(true);

    //setup memory
    memory->write(0x18, counter++); //operation(CLC4)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testClear(): test case CLC4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: CLD4 - decimal clear
     *Expected result: all false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setD(true);

    //setup memory
    memory->write(0xD8, counter++); //operation(CLD4)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testClears(): test case CLD4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: CLI4 - interupt clear
     *Expected result: all false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setI(true);

    //setup memory
    memory->write(0x58, counter++); //operation(CLI4)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getI() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testClears(): test case CLI4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: CLV4 - overflow clear
     *Expected result: all false
     */

    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setV(true);

    //setup memory
    memory->write(0xB8, counter++); //operation(CLV4)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getI() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testClears(): test case CLV4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testClears(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testBRK()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test BRK
     */

    //Test 1
    /*Operation: BRK interupt
     *Expected result: PC = 0, Stack contains 3 values(both parts of the pc, and the status reg),
     *B = 1, I = 1, Others = false
     *
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x00, counter++); //operation(BRK)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 3 &&
         cpu->_programCounter == 0x0 &&
         cpu->_status->getI() == true &&
         cpu->_status->getB() == true &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testBRK(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //test stack
    Stack copy = *cpu->_stack;
    byte expected[] = {0x30, 0x2, 0x6};
    unsigned int size = copy.size();

    for(unsigned int i = 0; i < size; i++)
    {
        byte value = copy[i];

        if(value != expected[i])
        {
            cout << "testBRK(): test case 1 failed! {Stack related: value at " << i << " expected as 0x" << std::hex << (int)expected[i] << "}" << endl;
            cpu->status("TEST CPU STATUS");

            //free resources
            delete cpu;

            return false;
        }
    }

    //all tests passed
    cout << "testBRK(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testCMP()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test CMP with A > O
     *2 - Test CMP with A = O
     *3 - Test CMP with A < 0
     */

    //Test 1
    /*Operation: CMP 10(A) with 5(O)
     *Expected result: C = 1, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x0A;

    //setup memory
    memory->write(0xC9, counter++); //operation(CMP)
    memory->write(0x05, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true))
    {
        cout << "testCMP(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: CMP 10(A) with 10(O)
     *Expected result: C = 1, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x0A;

    //setup memory
    memory->write(0xC9, counter++); //operation(CMP)
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true))
    {
        cout << "testCMP(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: CMP 10(A) with 11(O)
     *Expected result: S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x0A;

    //setup memory
    memory->write(0xC9, counter++); //operation(CMP)
    memory->write(0x0B, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false))
    {
        cout << "testCMP(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testCMP(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testCPX()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test CPX with X > O
     *2 - Test CPX with X = O
     *3 - Test CPX with X < 0
     */

    //Test 1
    /*Operation: CPX 10(X) with 5(O)
     *Expected result: C = 1, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0xE0, counter++); //operation(CPX)
    memory->write(0x05, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true))
    {
        cout << "testCPX(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: CPX 10(X) with 10(O)
     *Expected result: C = 1, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0xE0, counter++); //operation(CPX)
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true))
    {
        cout << "testCPX(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: CPX 10(X) with 11(O)
     *Expected result: S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0xE0, counter++); //operation(CPX)
    memory->write(0x0B, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false))
    {
        cout << "testCPX(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testCPX(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testCPY()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test CPY with Y > O
     *2 - Test CPY with Y = O
     *3 - Test CPY with Y < 0
     */

    //Test 1
    /*Operation: CPY 10(Y) with 5(O)
     *Expected result: C = 1, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0xC0, counter++); //operation(CPY)
    memory->write(0x05, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true))
    {
        cout << "testCPY(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: CPY 10(Y) with 10(O)
     *Expected result: C = 1, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0xC0, counter++); //operation(CPY)
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true))
    {
        cout << "testCPY(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: CPY 10(Y) with 11(O)
     *Expected result: S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0xC0, counter++); //operation(CPY)
    memory->write(0x0B, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false))
    {
        cout << "testCPY(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testCPY(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testDEC()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test DEC with value 0x0A, addressing mode: zero page
     *2 - Test DEC with value 0x01, addressing mode: zero page
     *3 - Test DEC with value 0x00, addressing mode: indexed X
     */

    //Test 1
    /*Operation: DEC 0x0A
     *Expected result: value @ Memory = 0x09, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xC6, counter++); //operation(DEC)
    memory->write(0x05, counter++);
    memory->write(0x0A, 0x05);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(0x05) == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEC(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: DEC 0x01
     *Expected result: value @ Memory = 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup memory
    memory->write(0xC6, counter++); //operation(DEC)
    memory->write(0x05, counter++);
    memory->write(0x01, 0x05);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(0x05) == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEC(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: DEC 0x00
     *Expected result: value @ Memory = 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x01;

    //setup memory
    memory->write(0xDE, counter++); //operation(DEC)
    memory->write(0x04, counter++);
    memory->write(0x00, 0x05);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(0x05) == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEC(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testDEC(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testDEX()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test DEX with value 0x0A
     *2 - Test DEX with value 0x01
     *3 - Test DEX with value 0x00
     */

    //Test 1
    /*Operation: DEX 0x0A
     *Expected result: x = 0x09, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0xCA, counter++); //operation(DEX)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEX(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: DEX 0x01
     *Expected result: x = 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x01;

    //setup memory
    memory->write(0xCA, counter++); //operation(DEX)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEX(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: DEX 0x00
     *Expected result: x = 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x00;

    //setup memory
    memory->write(0xCA, counter++); //operation(DEX)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEX(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testDEX(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testDEY()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test DEY with value 0x0A
     *2 - Test DEY with value 0x01
     *3 - Test DEY with value 0x00
     */

    //Test 1
    /*Operation: DEY 0x0A
     *Expected result: y = 0x09, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0x88, counter++); //operation(DEY)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0x09 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEY(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: DEY 0x01
     *Expected result: y = 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x01;

    //setup memory
    memory->write(0x88, counter++); //operation(DEY)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEY(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: DEY 0x00
     *Expected result: y = 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x00;

    //setup memory
    memory->write(0x88, counter++); //operation(DEY)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testDEY(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testDEY(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testEOR()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test EORing two positive numbers together to yield a positive result
     *2 - Test EORing a number that yields a negative result(S flag)
     *3 - Test EORing a 0 + 0 to yield a 0 result(Z flag)
     */

    //Test 1
    /*Operation: 0xFF /\ 0xA0
     *Expected result: 0x5F, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xFF;

    //setup memory
    memory->write(0x49, counter++); //EOR
    memory->write(0xA0, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x5F &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testEOR(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 0x9B /\ 0x1F
     *Expected result: 0x84, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x9B;

    //setup memory
    memory->write(0x49, counter++); //EOR
    memory->write(0x1F, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x84 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testEOR(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0x9B /\ 0x9B
     *Expected result: 0x00, Z = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x9B;

    //setup memory
    memory->write(0x49, counter++); //EOR
    memory->write(0x9B, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testEOR(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testEOR(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testINC()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test INC with value 0x0A, addressing mode: zero page
     *2 - Test INC with value 0x01, addressing mode: zero page
     *3 - Test INC with value 0x00, addressing mode: indexed X
     */

    //Test 1
    /*Operation: INC 0x0A
     *Expected result: value @ Memory = 0x0B, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xE6, counter++); //operation(INC)
    memory->write(0x05, counter++);
    memory->write(0x0A, 0x05);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(0x05) == 0x0B &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINC(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: INC 0xFF
     *Expected result: value @ Memory = 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup memory
    memory->write(0xE6, counter++); //operation(INC)
    memory->write(0x05, counter++);
    memory->write(0xFF, 0x05);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(0x05) == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINC(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: INC 0xFE
     *Expected result: value @ Memory = 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x01;

    //setup memory
    memory->write(0xFE, counter++); //operation(INC)
    memory->write(0x04, counter++);
    memory->write(0xFE, 0x05);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(memory->read(0x05) == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINC(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testINC(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testINX()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test INX with value 0x0A
     *2 - Test INX with value 0x01
     *3 - Test INX with value 0x00
     */

    //Test 1
    /*Operation: INX 0x0A
     *Expected result: x = 0x0B, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0x0A;

    //setup memory
    memory->write(0xE8, counter++); //operation(INX)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0x0B &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINX(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: INX 0xFF
     *Expected result: x = 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0xFF;

    //setup memory
    memory->write(0xE8, counter++); //operation(INX)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINX(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: INX 0xFE
     *Expected result: x = 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_x = 0xFE;

    //setup memory
    memory->write(0xE8, counter++); //operation(INX)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINX(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testINX(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testINY()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test INY with value 0x0A
     *2 - Test INY with value 0x01
     *3 - Test INY with value 0x00
     */

    //Test 1
    /*Operation: INY 0x0A
     *Expected result: y = 0x0B, others = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0x0A;

    //setup memory
    memory->write(0xC8, counter++); //operation(INY)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0x0B &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINY(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: INY 0xFF
     *Expected result: y = 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0xFF;

    //setup memory
    memory->write(0xC8, counter++); //operation(INY)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINY(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: INY 0xFE
     *Expected result: y = 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_y = 0xFE;

    //setup memory
    memory->write(0xC8, counter++); //operation(INY)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testINY(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testINY(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testJMP()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test JMP2 with value 0x0AAB
     *2 - Test JMP8 with value 0x0AAB->OxFFA1
     */

    //Test 1
    /*Operation: JMP2 0x0AAB
     *Expected result: pc = 0x0AAB, status bits = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x4C, counter++); //operation(JMP)
    memory->write(0xAB, counter++);
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x0AAB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testJMP(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: JMP8 0x0AAB->0xFFA1
     *Expected result: pc = 0xFFA1
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x6C, counter++); //operation(JMP)
    memory->write(0xAB, counter++);
    memory->write(0x0A, counter++);
    memory->write(0xA1, 0x0AAB);
    memory->write(0xFF, 0x0AAB + 1);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0xFFA1 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testJMP(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testJMP(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testJSR_RTS()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test JSR with value 0x0AAB
     *2 - Test RTS
     */

    //Test 1
    /*Operation: JSR 0x0AAB
     *Expected result: pc = 0x0AAB, stack count = 2, status bits = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x20, counter++); //operation(JSR)
    memory->write(0xAB, counter++);
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 2 &&
         cpu->_programCounter == 0x0AAB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testJSR_RTS(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //test stack
    Stack copy = *cpu->_stack;
    byte expected[] = {0x2, 0x6};
    unsigned int size = copy.size();

    for(unsigned int i = 0; i < size; i++)
    {
        byte value = copy[i];

        if(value != expected[i])
        {
            cout << "testJSR_RTS(): test case 1 failed! {Stack related: value at " << i << " expected as 0x" << std::hex << (int)expected[i] << "}" << endl;
            cpu->status("TEST CPU STATUS");

            //free resources
            delete cpu;

            return false;
        }
    }

    //Test 2
    /*Operation: RTS
     *Expected result: pc = 0x603
     */
    //reset variables(NO reset, as we want the last test state...)
    operations = 1;

    //setup registers

    //setup memory
    memory->write(0x60, 0x0AAB); //operation(RTS)

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 0 &&
         cpu->_programCounter == 0x603 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testJSR_RTS(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testJSR_RTS(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testLDA1()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test loading a positive number(lt 127)
     *2 - Test loading zero
     *3 - Test loading a negative number
     */

    //Test 1
    /*Operation: load 10
     *Expected result: 10, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA9, counter++);
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x0A &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDA1(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: load 0
     *Expected result: 0, Status: Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA9, counter++);
    memory->write(0x00, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDA1(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: load 200
     *Expected result: 200, Status: S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA9, counter++);
    memory->write(0xC8, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0xC8 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDA1(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testLDA1(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testLDX()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test loading a positive number(lt 127)
     *2 - Test loading zero
     *3 - Test loading a negative number
     */

    //Test 1
    /*Operation: load 10
     *Expected result: 10, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA2, counter++);
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0x0A &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDX(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: load 0
     *Expected result: 0, Status: Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA2, counter++);
    memory->write(0x00, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDX(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: load 200
     *Expected result: 200, Status: S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA2, counter++);
    memory->write(0xC8, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_x == 0xC8 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDX(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testLDX(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testLDY()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test loading a positive number(lt 127)
     *2 - Test loading zero
     *3 - Test loading a negative number
     */

    //Test 1
    /*Operation: load 10
     *Expected result: 10, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA0, counter++);
    memory->write(0x0A, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0x0A &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDY(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: load 0
     *Expected result: 0, Status: Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA0, counter++);
    memory->write(0x00, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDY(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: load 200
     *Expected result: 200, Status: S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xA0, counter++);
    memory->write(0xC8, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_y == 0xC8 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLDY(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testLDY(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testLSR()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test loading number
     *2 - Test loading zero
     *3 - Test causing a carry
     */

    //Test 1
    /*Operation: LSR 0x0E
     *Expected result: 0x07, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x0E;

    //setup memory
    memory->write(0x4A, counter++); //LSR operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x07 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testLSR(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: LSR 1
     *Expected result: 0, Status: Z = 1, C = 1 others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;

    //setup memory
    memory->write(0x4A, counter++); //LSR operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testLSR(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testLSR(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testORA()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test ORAing two positive numbers together to yield a positive result
     *2 - Test ORAing a number that yields a negative result(S flag)
     *3 - Test ORAing a 0 + 0 to yield a 0 result(Z flag)
     */

    //Test 1
    /*Operation: 0xB | 0xA
     *Expected result: 0xB, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xB;

    //setup memory
    memory->write(0x09, counter++); //ORA
    memory->write(0xA, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0xB &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testORA(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 0x9B | 0x1F
     *Expected result: 0x9F, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x9B;

    //setup memory
    memory->write(0x09, counter++); //ORA
    memory->write(0x1F, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x9F &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testORA(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0x0 | 0x0
     *Expected result: 0x00, Z = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;

    //setup memory
    memory->write(0x09, counter++); //ORA
    memory->write(0x00, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testORA(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testORA(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testPHA()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test pushing accumulator
     */

    //Test 1
    /*Operation: Push acc
     *Expected result: stack size = 1, top = 0xA, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xA;

    //setup memory
    memory->write(0x48, counter++); //PHA

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 1 &&
         cpu->_accumulator == 0xA &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testPHA(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //test stack
    Stack copy = *cpu->_stack;
    byte expected[] = {0xA};
    unsigned int size = copy.size();

    for(unsigned int i = 0; i < size; i++)
    {
        byte value = copy[i];

        if(value != expected[i])
        {
            cout << "testPHA(): test case 1 failed! {Stack related: value at " << i << " expected as 0x" << std::hex << (int)expected[i] << "}" << endl;
            cpu->status("TEST CPU STATUS");

            //free resources
            delete cpu;

            return false;
        }
    }

    //all tests passed
    cout << "testPHA(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testPHP()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test pushing status
     */

    //Test 1
    /*Operation: Push status
     *Expected result: stack size = 1, top = 0x22, Z = true, others false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setZ(true);

    //setup memory
    memory->write(0x08, counter++); //PHP

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 1 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testPHP(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //test stack
    Stack copy = *cpu->_stack;
    byte expected[] = {0x22};
    unsigned int size = copy.size();

    for(unsigned int i = 0; i < size; i++)
    {
        byte value = copy[i];

        if(value != expected[i])
        {
            cout << "testPHP(): test case 1 failed! {Stack related: value at " << i << " expected as 0x" << std::hex << (int)expected[i] << "}" << endl;
            cpu->status("TEST CPU STATUS");

            //free resources
            delete cpu;

            return false;
        }
    }

    //all tests passed
    cout << "testPHP(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testPLA()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test pulling stack
     *2 - Test pulling stack as 0
     *3 - Test pulling stack as negative
     */

    //Test 1
    /*Operation: pull stack
     *Expected result: stack size = 0, acc = 0xA, all status = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_stack->push(0xA);

    //setup memory
    memory->write(0x68, counter++); //PLA

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 0 &&
         cpu->_accumulator == 0xA &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testPLA(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: pull 0x00 stack
     *Expected result: 0x00, Z = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_stack->push(0x0);

    //setup memory
    memory->write(0x68, counter++); //PLA

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 0 &&
         cpu->_accumulator == 0x0 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testPLA(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: pull 0xFF stack
     *Expected result: 0xFF, S = 1, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_stack->push(0xFF);

    //setup memory
    memory->write(0x68, counter++); //PLA

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 0 &&
         cpu->_accumulator == 0xFF &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testPLA(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testPLA(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testPLP()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test pulling status from stack
     */

    //Test 1
    /*Operation: pull 0x22 from stack
     *Expected result: stack size = 0, Z = true, rest = false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_stack->push(0x22);

    //setup memory
    memory->write(0x28, counter++); //PLP

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_stack->size() == 0 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testPLP(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testPLP(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testROL()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test loading number
     *2 - Test loading zero & carry
     *3 - Test causing a carry
     *4 - Test caussing a 'roll' with a carry
     */

    //Test 1
    /*Operation: ROL 0x08
     *Expected result: 0x10, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x08;

    //setup memory
    memory->write(0x2A, counter++); //ROL operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x10 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testROL(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: ROL 0x80
     *Expected result: 0, Status: Z = 1, C = 1 others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x80;

    //setup memory
    memory->write(0x2A, counter++); //ROL operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testROL(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: ROL 0x00 with C = 1
     *Expected result: 0x01, Status: others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x00;
    cpu->_status->setC(true);

    //setup memory
    memory->write(0x2A, counter++); //ROL operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x01 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testROL(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testROL(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testROR()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test loading number
     *2 - Test loading zero & carry
     *3 - Test causing a carry
     *4 - Test caussing a 'roll' with a carry
     */

    //Test 1
    /*Operation: ROR 0x40
     *Expected result: 0x20, All status bits false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x40;

    //setup memory
    memory->write(0x6A, counter++); //ROR operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x20 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testROR(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: ROR 0x01
     *Expected result: 0, Status: Z = 1, C = 1 others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x01;

    //setup memory
    memory->write(0x6A, counter++); //ROR operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testROR(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: ROR 0x80 with C = 1
     *Expected result: 0xC0, Status: S = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x80;
    cpu->_status->setC(true);

    //setup memory
    memory->write(0x6A, counter++); //ROR operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0xC0 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testROR(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testROR(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testRTI()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Restore cpu state
     */

    //Test 1
    /*Operation: RTI
     *Expected result: PC = 0x0A, Z = true, others false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_stack->push(0x0A);
    cpu->_stack->push(0x22);

    //setup memory
    memory->write(0x40, counter++); //RTI operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_programCounter == 0x0A &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testRTI(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testRTI(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testSBC()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - Test subtracting two positive numbers together to yield a positive result
     *2 - Test subtracting a number to 127 that yields a negative result(S flag)
     *3 - Test subtracting a 0 + 0 to yield a 0 result(Z flag)
     *4 - Test subtracting two numbers that overflow(go below 0)
     */

    //Test 1
    /*Operation: 0xED - 0x7F
     *Expected result: ACC = 0x6D, C = true, others false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xED;

    //setup memory
    memory->write(0xE9, counter++); //SBC operation
    memory->write(0x7F, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x6D &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testSBC(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 2
    /*Operation: 0xA7 - 0x1E
     *Expected result: 0x88, Status: C = True, S = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0xA7;

    //setup memory
    memory->write(0xE9, counter++); //SBC operation
    memory->write(0x1E, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x88 &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testSBC(): test case 2 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 3
    /*Operation: 0x00 - 0x00
     *Expected result: 0x00, Status: C = True, Z = true, others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_status->setC(true);
    cpu->_accumulator = 0x00;

    //setup memory
    memory->write(0xE9, counter++); //SBC operation
    memory->write(0x00, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0x00 &&
         cpu->_status->getS() == false &&
         cpu->_status->getZ() == true &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testSBC(): test case 3 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //Test 4
    /*Operation: 0x50 - 0x64
     *Expected result: 0xEB, Status:V = true, S = true others = false
     */
    //reset variables
    cpu->reset();
    counter = start;
    operations = 1;
    cpu->setPC(start);

    //setup registers
    cpu->_accumulator = 0x50;

    //setup memory
    memory->write(0xE9, counter++); //SBC operation
    memory->write(0x64, counter++);

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_accumulator == 0xEB &&
         cpu->_status->getS() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == true))
    {
        cout << "testSBC(): test case 4 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testSBC(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testSEC()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - set carry true
     */

    //Test 1
    /*Operation: SEC
     *Expected result: C = true, others false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x38, counter++); //SEC operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getS() == false &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == true &&
         cpu->_status->getV() == false))
    {
        cout << "testSEC(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testSEC(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testSED()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - set carry true
     */

    //Test 1
    /*Operation: SED
     *Expected result: D = true, others false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0xF8, counter++); //SED operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getD() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testSED(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testSED(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testSEI()
{
    //Locals
    BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
    MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
    unsigned short start = 0x600;
    unsigned short counter = start;
    int operations = 0; //SET THIS WHEN SETTING UP CASES

    /*Test Cases:
     *1 - set carry true
     */

    //Test 1
    /*Operation: SEI
     *Expected result: I = true, others false
     */
    operations = 1;
    cpu->setPC(start);

    //setup registers

    //setup memory
    memory->write(0x78, counter++); //SEI operation

    //run operations
    for(int i = 0; i < operations; i++)
        cpu->runNext(false);

    //Check if result differs from expected
    if(!(cpu->_status->getI() == true &&
         cpu->_status->getZ() == false &&
         cpu->_status->getC() == false &&
         cpu->_status->getV() == false))
    {
        cout << "testSEI(): test case 1 failed!" << endl;
        cpu->status("TEST CPU STATUS");

        //free resources
        delete cpu;

        return false;
    }

    //all tests passed
    cout << "testSEI(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testSTA()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - load acc to memory
	*/

	//Test 1
	/*Operation: STA 0xFA
	*Expected result: mem @ 0xFA = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0x0A;

	//setup memory
	memory->write(0x85, counter++); //STA operation
	memory->write(0xFA, counter++);

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(memory->read(0xFA) == 0x0A &&
		cpu->_status->getI() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testSTA(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testSTA(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testSTX()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - load x to memory
	*/

	//Test 1
	/*Operation: STX 0xFA
	*Expected result: mem @ 0xFA = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_x = 0x0A;

	//setup memory
	memory->write(0x86, counter++); //STX operation
	memory->write(0xFA, counter++);

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(memory->read(0xFA) == 0x0A &&
		cpu->_status->getI() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testSTX(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testSTX(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testSTY()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - load y to memory
	*/

	//Test 1
	/*Operation: STY 0xFA
	*Expected result: mem @ 0xFA = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_y = 0x0A;

	//setup memory
	memory->write(0x84, counter++); //STY operation
	memory->write(0xFA, counter++);

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(memory->read(0xFA) == 0x0A &&
		cpu->_status->getI() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testSTY(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testSTY(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testTAX()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - Transfer A -> X with positive number
	*2 - Transfer A -> X with negative number
	*3 - Transfer A -> X with 0
	*/

	//Test 1
	/*Operation: TAX
	*Expected result: x = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0x0A;

	//setup memory
	memory->write(0xAA, counter++); //TAX operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_x == 0x0A &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTAX(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 2
	/*Operation: TAX
	*Expected result: x =  0xFA, S = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0xFA;

	//setup memory
	memory->write(0xAA, counter++); //TAX operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_x == 0xFA &&
		cpu->_status->getS() == true &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTAX(): test case 2 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 3
	/*Operation: TAX
	*Expected result: x =  0x00, Z = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0x00;

	//setup memory
	memory->write(0xAA, counter++); //TAX operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_x == 0x00 &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == true &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTAX(): test case 3 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testTAX(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testTAY()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - Transfer A -> Y with positive number
	*2 - Transfer A -> Y with negative number
	*3 - Transfer A -> Y with 0
	*/

	//Test 1
	/*Operation: TAY
	*Expected result: x = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0x0A;

	//setup memory
	memory->write(0xA8, counter++); //TAY operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_y == 0x0A &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTAY(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 2
	/*Operation: TAY
	*Expected result: x =  0xFA, S = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0xFA;

	//setup memory
	memory->write(0xA8, counter++); //TAY operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_y == 0xFA &&
		cpu->_status->getS() == true &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTAY(): test case 2 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 3
	/*Operation: TAY
	*Expected result: x =  0x00, Z = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_accumulator = 0x00;

	//setup memory
	memory->write(0xA8, counter++); //TAY operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_y == 0x00 &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == true &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTAY(): test case 3 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testTAY(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testTSX()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - Transfer SR -> X with positive number
	*2 - Transfer SR -> X with negative number
	*3 - Transfer SR -> X with 0
	*/

	//Test 1
	/*Operation: TSX
	*Expected result: x = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_stackPointer = 0x0A;

	//setup memory
	memory->write(0xBA, counter++); //TSX operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_x == 0x0A &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTSX(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 2
	/*Operation: TSX
	*Expected result: x =  0xFA, S = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_stackPointer = 0xFA;

	//setup memory
	memory->write(0xBA, counter++); //TSX operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_x == 0xFA &&
		cpu->_status->getS() == true &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTSX(): test case 2 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 3
	/*Operation: TSX
	*Expected result: x =  0x00, Z = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_stackPointer = 0x00;

	//setup memory
	memory->write(0xBA, counter++); //TSX operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_x == 0x00 &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == true &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTSX(): test case 3 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testTSX(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testTXA()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - Transfer X -> A with positive number
	*2 - Transfer X -> A with negative number
	*3 - Transfer X -> A with 0
	*/

	//Test 1
	/*Operation: TXA
	*Expected result: acc = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_x = 0x0A;

	//setup memory
	memory->write(0x8A, counter++); //TXA operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_accumulator == 0x0A &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTXA(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 2
	/*Operation: TXA
	*Expected result: acc =  0xFA, S = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_x = 0xFA;

	//setup memory
	memory->write(0x8A, counter++); //TXA operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_accumulator == 0xFA &&
		cpu->_status->getS() == true &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTXA(): test case 2 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 3
	/*Operation: TXA
	*Expected result: acc =  0x00, Z = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_x = 0x00;

	//setup memory
	memory->write(0x8A, counter++); //TXA operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_accumulator == 0x00 &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == true &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTXA(): test case 3 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testTXA(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testTXS()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - Transfer X -> SR
	*/

	//Test 1
	/*Operation: TXS
	*Expected result: SR = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_x = 0x0A;

	//setup memory
	memory->write(0x9A, counter++); //TXS operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_stackPointer == 0x0A &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTXS(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testTXS(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testTYA()
{
	//Locals
	BasicMemory* memory = new BasicMemory(MEMORY_SIZE);
	MOS6502CPU* cpu = new MOS6502CPU(2, memory, true);
	unsigned short start = 0x600;
	unsigned short counter = start;
	int operations = 0; //SET THIS WHEN SETTING UP CASES

	/*Test Cases:
	*1 - Transfer Y -> A with positive number
	*2 - Transfer Y -> A with negative number
	*3 - Transfer Y -> A with 0
	*/

	//Test 1
	/*Operation: TYA
	*Expected result: acc = 0x0A, others false
	*/
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_y = 0x0A;

	//setup memory
	memory->write(0x98, counter++); //TYA operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_accumulator == 0x0A &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTYA(): test case 1 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 2
	/*Operation: TYA
	*Expected result: acc =  0xFA, S = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_y = 0xFA;

	//setup memory
	memory->write(0x98, counter++); //TYA operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_accumulator == 0xFA &&
		cpu->_status->getS() == true &&
		cpu->_status->getZ() == false &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTYA(): test case 2 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//Test 3
	/*Operation: TYA
	*Expected result: acc =  0x00, Z = true, others = false
	*/
	//reset variables
	cpu->reset();
	counter = start;
	operations = 1;
	cpu->setPC(start);

	//setup registers
	cpu->_y = 0x00;

	//setup memory
	memory->write(0x98, counter++); //TYA operation

	//run operations
	for (int i = 0; i < operations; i++)
		cpu->runNext(false);

	//Check if result differs from expected
	if (!(cpu->_accumulator == 0x00 &&
		cpu->_status->getS() == false &&
		cpu->_status->getZ() == true &&
		cpu->_status->getC() == false &&
		cpu->_status->getV() == false))
	{
		cout << "testTYA(): test case 3 failed!" << endl;
		cpu->status("TEST CPU STATUS");

		//free resources
		delete cpu;

		return false;
	}

	//all tests passed
	cout << "testTYA(): all passed!" << endl;

	//free resources
	delete cpu;

	return true;
}

bool CpuTest::testBCDArithmetic()
{
    //adding
    //15(0x15) + 36(0x36) = 51(0x51)
    BCDResult result = Utility::addBCD(0x15, 0x36);

    if(result.result != 0x51)
    {
        cout << "Result: " << hex << (int)result.result << endl;
        cout << "testBCDArithmetic(): Test 1 Failed!" << endl;
        return false;
    }

    //61 + 40 = 101
    result = Utility::addBCD(0x61, 0x40);

    if(result.result != 0x01) //overflow and carry
    {
        cout << "Result: " << hex << (int)result.result << endl;
        cout << "testBCDArithmetic(): Test 2 Failed!" << endl;
    }

    cout << "testBCDArithmetic(): all passed!" << endl;
    return true;
}

void CpuTest::runTests()
{
    int testsFailed = 0;

    cout << "Running tests..." << endl;

    if(!testADC1()) testsFailed++;
    if(!testADC3()) testsFailed++;
    if(!testADC7()) testsFailed++;
    if(!testADC2()) testsFailed++;
    if(!testADC6_X()) testsFailed++;
    if(!testADC6_Y()) testsFailed++;
    if(!testADC9()) testsFailed++;
    if(!testADC10()) testsFailed++;
    cout << endl;

    if(!testAND()) testsFailed++;
    cout << endl;

    if(!testASL5()) testsFailed++;
    if(!testASL3()) testsFailed++;
    cout << endl;

    if(!testBCC()) testsFailed++;
    if(!testBCS()) testsFailed++;
    if(!testBEQ()) testsFailed++;
    if(!testBMI()) testsFailed++;
    if(!testBNE()) testsFailed++;
    if(!testBPL()) testsFailed++;
    if(!testBVC()) testsFailed++;
    if(!testBVS()) testsFailed++;
    cout << endl;

    if(!testBIT()) testsFailed++;
    cout << endl;

    if(!testClears()) testsFailed++;
    cout << endl;

    if(!testBRK()) testsFailed++;
    cout << endl;

    if(!testCMP()) testsFailed++;
    if(!testCPX()) testsFailed++;
    if(!testCPY()) testsFailed++;
    cout << endl;

    if(!testDEC()) testsFailed++;
    if(!testDEX()) testsFailed++;
    if(!testDEY()) testsFailed++;
    cout << endl;

    if(!testEOR()) testsFailed++;
    cout << endl;

    if(!testINC()) testsFailed++;
    if(!testINX()) testsFailed++;
    if(!testINY()) testsFailed++;
    cout << endl;

    if(!testJMP()) testsFailed++;
    if(!testJSR_RTS()) testsFailed++;
    cout << endl;

    if(!testLDA1()) testsFailed++;
    if(!testLDX()) testsFailed++;
    if(!testLDY()) testsFailed++;
    cout << endl;

    if(!testLSR()) testsFailed++;
    cout << endl;

    if(!testORA()) testsFailed++;
    cout << endl;

    if(!testPHA()) testsFailed++;
    if(!testPHP()) testsFailed++;
    if(!testPLA()) testsFailed++;
    if(!testPLP()) testsFailed++;
    cout << endl;

    if(!testROL()) testsFailed++;
    if(!testROR()) testsFailed++;
    cout << endl;

    if(!testRTI()) testsFailed++;
    cout << endl;

    if(!testSBC()) testsFailed++;
    cout << endl;

    if(!testSEC()) testsFailed++;
    if(!testSED()) testsFailed++;
    if(!testSEI()) testsFailed++;
	cout << endl;

	if(!testSTA()) testsFailed++;
	if(!testSTX()) testsFailed++;
	if(!testSTY()) testsFailed++;
	cout << endl;

	if(!testTAX()) testsFailed++;
	if(!testTAY()) testsFailed++;
	if(!testTSX()) testsFailed++;
	if(!testTXA()) testsFailed++;
	if(!testTXS()) testsFailed++;
	if(!testTYA()) testsFailed++;
	cout << endl;

	if(!testBCDArithmetic()) testsFailed++;

    cout << "Tests failed: " << testsFailed << endl;
}
