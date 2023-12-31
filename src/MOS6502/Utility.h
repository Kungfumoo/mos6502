//Author: Aaron Winter
//Date: 1/11/2014
/*Description: Class that contains static methods to perform general tasks
 */

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <bitset>

typedef unsigned char byte;

namespace MOS_6502
{
    //Struct used to store arithmetic results
    struct BCDResult
    {
        byte result;
        bool carry;
        bool overflow;
    };

    class Utility
    {
    public:
        static const byte NINES_COMPLIMENT;

        //--Static General Methods:
        static std::bitset<8> toBinary(byte number);
        static byte toByte(std::bitset<8>& bits);
        static byte toByte(std::bitset<8>& bits, byte start, byte finish); //define bits to look at with start and finish

        //toBCD should ONLY be used to create human readable displays, NOT for arithmatic!!!
        static byte toBCD(byte number);
        static byte toBCD(std::bitset<8>& bits);
        static BCDResult addBCD(std::bitset<8>& num1, std::bitset<8>& num2);
        static BCDResult addBCD(byte num1, byte num2);
        static BCDResult subtractBCD(std::bitset<8>& num1, std::bitset<8>& num2);
        static BCDResult subtractBCD(byte num1, byte num2);
    };
}
#endif
