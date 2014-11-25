//Author: Aaron Winter
//Date: 1/11/2014
/*Description: Class that contains static methods to perform general tasks
 */

#include <bitset>

typedef unsigned char byte;

namespace MOS_6502
{
    class Utility
    {
    public:
        //--Static General Methods:
        static std::bitset<8> toBinary(byte number);
        static byte toByte(std::bitset<8>& bits);
    };
}
