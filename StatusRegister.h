//Author: Aaron Winter
//Date: 7/9/2014
/*Description:
 *MOS 6502 status register:
 *  |7|6|5|4|3|2|1|0
 *  |S|V| |B|D|I|Z|C
 *
 * -C = Carry flag
 * -Z = Zero flag
 * -I = Interupt flag
 * -D = Decimal mode flag
 * -B = set during a software interupt
 * -Bit 5 = not used, set to 1
 * -V = Overflow flag
 * -S = Sign flag
 */

#include <bitset>
#include <string>

typedef unsigned char byte;

namespace MOS_6502
{
    class StatusRegister
    {
        //Variables
        std::bitset<8> _register;

    public:
        //--Acessors/Mutatos
        bool getC();
        bool getZ();
        bool getI();
        bool getD();
        bool getB();
        bool getV();
        bool getS();

        void setC(bool value);
        void setZ(bool value);
        void setI(bool value);
        void setD(bool value);
        void setB(bool value);
        void setV(bool value);
        void setS(bool value);

        //--General
        void reset();

        //Returns a binary string representation of the register
        std::string toString();

        //Returns the status register as a byte.
        byte toByte();

        //builds status register based on value
        void fromByte(byte value);

        //--Constructors
        //Constructor
        StatusRegister();

        //Destructor
        ~StatusRegister();
    };
}
