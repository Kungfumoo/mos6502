#ifndef _AT2600_MEMORY_H_
#define _AT2600_MEMORY_H_

#include <vector>
#include "BasicMemory.h"

/* 
 * NOTES:
 * Atari2600 memory can only address 8192 bytes as it uses the reduced 6507 (6502 but missing memory pins)
 * Atari2600 memory structure is as follows:
 *      0x0000 ~ 0x007F = TIA Registers
 *      0x0080 ~ 0x00FF = RAM (128 bytes)
 *      0x0200 ~ 0x02FF = RIOT Registers
 *      0x1000 ~ 0x1FFF = ROM
 */

namespace Atari2600
{
    class Memory : public MOS_6502::BasicMemory
    {
        static const unsigned short MAX_SIZE_BYTES;
        static const unsigned short ROM_START_ADDR;

    public:
        void loadRom(std::vector<byte> program);

        Memory();
    };
}

#endif