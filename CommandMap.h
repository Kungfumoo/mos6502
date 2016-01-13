//Author: Aaron Winter
//Date: 12/1/2016
/*Description:
 * Pre-built map of commands
 */

#ifndef _COMMAND_MAP_H_
#define _COMMAND_MAP_H_

#include <unordered_map>
#include <vector>
#include <string>

namespace MOS_6502
{
namespace CompilerAssets
{
    class CommandMap : public std::unordered_map<char, std::vector<std::string>>
    {
    public:
        //--Constructors
        CommandMap();
    };
}
}
#endif // _COMMAND_MAP_H_
