//Author: Aaron Winter
//Date: 9/9/2014
/*Description:
* Class representing Basic RAM for the CPU.
*/

#include "Memory.h"

#ifndef _BASIC_MEMORY_H_
#define _BASIC_MEMORY_H_
namespace MOS_6502
{
	class BasicMemory : public Memory
	{
	public:
		//--General Methods
		byte read(unsigned short address) override; //read from said location in memory
		void write(byte value, unsigned short address) override; //write to said location in memory

		//--Constructors
		//Constructor
		BasicMemory();
		BasicMemory(unsigned int bytes);
	};
}
#endif