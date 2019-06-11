#ifndef _AT2600_TELEVISION_INTERFACE_ADAPTER_H_
#define _AT2600_TELEVISION_INTERFACE_ADAPTER_H_

typedef unsigned char byte;

namespace Atari2600
{
    //forward decs
    namespace DisplayAdapter
    {
        class DisplayAdapterInterface;
    }

    class Memory;
    //end forward decs

    class TelevisionInterfaceAdapter
    {
        static const byte CLOCKS_PER_SCANLINE;
        static const unsigned short MAX_SCANLINES;
        static const byte VERTICAL_PICTURE_THRESHOLD;
        static const byte HORIZONTAL_PICTURE_THRESHOLD;

        DisplayAdapter::DisplayAdapterInterface* _displayAdapter;
        Memory* _memory;
        byte _clockCounter;
        byte _vScanlineCounter;

    public:
        static const float CLOCK_SPEED;

        bool runCycle(); //run a cycle of tia, return true to break CPU syncronisation (for things like WSYNC)

        TelevisionInterfaceAdapter(DisplayAdapter::DisplayAdapterInterface* displayAdapter, Memory* memory);
        ~TelevisionInterfaceAdapter();
    };
}

#endif