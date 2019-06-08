#ifndef _ATARI_2600_DISPLAY_ADAPTER_INTERFACE_H_
#define _ATARI_2600_DISPLAY_ADAPTER_INTERFACE_H_

typedef unsigned char byte;

namespace Atari2600
{
namespace DisplayAdapter
{
    struct Position
    {
        unsigned int x;
        unsigned int y;
    };

    struct Colour
    {
        byte r;
        byte g;
        byte b;
    };

    class DisplayAdapterInterface
    {
    public:
        virtual void renderPixel(Position& pos, Colour& colour) = 0;
    };
}
}

#endif