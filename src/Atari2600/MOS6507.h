#ifndef _ATARI_2600_MOS_6507
#define _ATARI_2600_MOS_6507

#include "../MOS6502/MOS6502CPU.h"

namespace Atari2600
{
    //forward dec
    class Memory;

    class MOS6507 : public MOS_6502::MOS6502CPU
    {
    protected:
        //--ADDRESSING MODE METHODS
        unsigned short getAbsolute() override;

    public:
        static const unsigned short ADDRESS_LIMIT;

        //--Constructors:
        /*Constructor 1
            *clockSpeedMhz = the clockspeed that this should operate on in Mhz(max is 2)
            *memory = A pointer to an created Memory object(represents RAM)
            */
        MOS6507(float clockSpeedMhz, Memory* memory, bool debug);

        //Destructor
        ~MOS6507();
    };
}

#endif