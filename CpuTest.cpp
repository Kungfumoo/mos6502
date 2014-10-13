//CpuTest class implementation
#include "CpuTest.h"
#include "MOS6502CPU.h"
#include "Memory.h"
#include "StatusRegister.h"
#include <iostream>

using namespace MOS_6502;
using namespace std;

//const definitions
const unsigned int CpuTest::MEMORY_SIZE = 64000;

//--Test Cases
bool CpuTest::testADC1()
{
    //Locals
    Memory* memory = new Memory(MEMORY_SIZE);
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

    //all tests passed
    cout << "testADC1(): all passed!" << endl;

    //free resources
    delete cpu;

    return true;
}

bool CpuTest::testADC3()
{
    //Locals
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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
    Memory* memory = new Memory(MEMORY_SIZE);
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

bool CpuTest::testLDA1()
{
    //Locals
    Memory* memory = new Memory(MEMORY_SIZE);
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

    if(!testLDA1()) testsFailed++;

    cout << "Tests failed: " << testsFailed << endl;
}
