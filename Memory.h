//Author: Aaron Winter
//Date: 9/9/2014
/*Description:
 * Abstract Class representing RAM for the CPU.
 * Use this to provide unique implementation for the platform you're emulating
 */

typedef unsigned char byte;

#ifndef _MEMORY_H_
#define _MEMORY_H_
namespace MOS_6502
{
    class Memory
    {
        //Variables
        unsigned int _size; //size of memory

	protected:
		byte* _memory; //byte array

    public:
		//Constants
		static const unsigned int MAX_SIZE_BYTES;

        //--Accessors
        unsigned int getSize();

        //--General Methods
		virtual byte read(unsigned short address) = 0; //read from said location in memory
		virtual void write(byte value, unsigned short address) = 0; //write to said location in memory
        void clear(); //flushes memory(do not use during a program)

        //--Constructors:
        //Constructor
        Memory(unsigned int bytes); //create memory with specified byte amount

        //Destructor
        virtual ~Memory();
    };
}
#endif
