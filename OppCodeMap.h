//Author: Aaron Winter
//Date: 12/1/2016
/*Description:
 * Pre-built map of oppcodes
 */

#ifndef _OPPCODE_MAP_H_
#define _OPPCODE_MAP_H_

#include <unordered_map>
#include <vector>
#include <string>

typedef unsigned char byte;

namespace MOS_6502
{
namespace CompilerAssets
{
    class OppCodeMap : public std::unordered_map<std::string, std::vector<byte>>
    {
    public:
        //--Constructors
        OppCodeMap();
    };
}
}
#endif // _OPPCODE_MAP_H_
