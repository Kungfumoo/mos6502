//Author: Aaron Winter
//Date: 24.2.2015
/*Description:
 *Represents the MOS6502 cpu stack.
 *256 bytes between 0x0100 ~ 0x01FF
 */

typedef unsigned char byte;

#ifndef _STACK_H_
#define _STACK_H_
namespace MOS_6502
{
    //forward decs
    class MOS6502CPU;

    class Stack
    {
        //Constants
        static byte TOP; //byte that represents the top value(used for reset and size calcs)

        //Variables
        MOS6502CPU* _cpu; //pointer to main cpu instance

    public:
        //--General Methods
        void push(byte data);
        byte pop();
        unsigned int size();
        void reset();

        //--Constructors
        //Constructor
        Stack(MOS6502CPU* cpu);
    };
}
#endif
