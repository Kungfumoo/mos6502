//Author: Aaron Winter
//Date: 9/9/2014
/*Description:
 * Class representing RAM for the CPU.
 */

typedef unsigned char byte;

#ifndef _MEMORY_H_
#define _MEMORY_H_
namespace MOS_6502
{
    class Memory
    {
        //Constants
        static const unsigned int MAX_SIZE_BYTES;

        //Variables
        byte* _memory; //byte array
        unsigned int _size; //size of memory

    public:
        //--Accessors
        unsigned int getSize();

        //--General Methods
        byte read(unsigned short address); //read from said location in memory
        void write(byte value, unsigned short address); //write to said location in memory
        void clear(); //flushes memory(do not use during a program)

        //--Constructors:
        //Constructor
        Memory(unsigned int bytes); //create memory with specified byte amount

        //Destructor
        ~Memory();
    };
}
#endif
