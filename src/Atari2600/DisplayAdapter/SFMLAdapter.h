#ifndef _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_
#define _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_

#include "DisplayAdapterInterface.h"

namespace Atari2600
{
namespace DisplayAdapter
{
    class SFMLAdapter : public DisplayAdapterInterface
    {
        public:
            virtual void renderPixel(Position& pos, Colour& colour);
            virtual void init();
    };
}
}

#endif