#ifndef _AT2600_TELEVISION_INTERFACE_ADAPTER_H_
#define _AT2600_TELEVISION_INTERFACE_ADAPTER_H_

typedef unsigned char byte;

namespace Atari2600
{
    //forward decs
    namespace DisplayAdapter
    {
        struct Colour;
        class DisplayAdapterInterface;
    }

    class Memory;
    //end forward decs

    class TelevisionInterfaceAdapter
    {
        static const byte CLOCKS_PER_SCANLINE;
        static const unsigned short MAX_SCANLINES;
        static const byte VERTICAL_PICTURE_THRESHOLD;
        static const byte VERTICAL_OVERSCAN_THRESHOLD;
        static const byte HORIZONTAL_PICTURE_THRESHOLD;

        DisplayAdapter::DisplayAdapterInterface* _displayAdapter;
        Memory* _memory;
        unsigned short _clockCounter;
        unsigned short _vScanlineCounter;

        //Converts the register colour value into an RBG colour (Based on NTSC)
        DisplayAdapter::Colour resolveColour(byte value);

        void renderScanline();

    public:
        static const float CLOCK_SPEED;

        bool runCycle(); //run a cycle of tia, return true to break CPU syncronisation (for things like WSYNC)

        TelevisionInterfaceAdapter(DisplayAdapter::DisplayAdapterInterface* displayAdapter, Memory* memory);
        ~TelevisionInterfaceAdapter();
    };
}

#endif