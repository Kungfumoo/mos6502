//Utility class implementation
#include "Utility.h"
#include "Exceptions.h"

using namespace MOS_6502;
using namespace std;

//--Public static methods

/*Imagine back to front:
 * |7|6|5|4|3|2|1|0
 *  0 0 0 0 0 0 0 0
 *
 * bit 1 in this representation is the same as index 1
 */
bitset<8> Utility::toBinary(byte number)
{
    bitset<8> binaryString;

    for(int i = 0; i < 8; i++)
    {
        binaryString[i] = (number % 2) > 0;
        number = number >> 1;
    }

    return binaryString;
}

byte Utility::toByte(bitset<8>& bits)
{
    byte value = 0;
    /*
     * |7|6|5|4|3|2|1|0
     * 128-64-32-16-8-4-2-1
     */

    value += (bits[0]) ? 1 : 0;
    value += (bits[1]) ? 2 : 0;
    value += (bits[2]) ? 4 : 0;
    value += (bits[3]) ? 8 : 0;
    value += (bits[4]) ? 16 : 0;
    value += (bits[5]) ? 32 : 0;
    value += (bits[6]) ? 64 : 0;
    value += (bits[7]) ? 128 : 0;

    return value;
}

byte Utility::toByte(std::bitset<8>& bits, byte start, byte finish)
{
    byte values[] = {1, 2, 4, 8, 16, 32, 64, 128};
    byte value = 0;

    if(start < 8 && finish < 8)
    {
        for(int i = start; i <= finish; i++)
            value += values[i];
    }
    else
        throw new BitLimitExceededException();

    return value;
}

byte Utility::toBCD(byte number)
{
    std::bitset<8> bits = Utility::toBinary(number);

    return Utility::toBCD(bits);
}

byte Utility::toBCD(bitset<8>& bits)
{
    byte rightNibble = Utility::toByte(bits, 0, 3);
    bits >>= 4;
    byte leftNibble = Utility::toByte(bits, 0, 3);

    return rightNibble * 10 + leftNibble;
}
