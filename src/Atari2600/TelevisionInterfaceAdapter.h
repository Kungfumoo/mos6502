#ifndef _AT2600_TELEVISION_INTERFACE_ADAPTER_H_
#define _AT2600_TELEVISION_INTERFACE_ADAPTER_H_

typedef unsigned char byte;

namespace Atari2600
{
    //forward dec
    class Memory;

    class TelevisionInterfaceAdapter
    {
        static const byte CLOCKS_PER_SCANLINE;
        
        Memory* _memory;
        byte _clockCounter;

    public:
        static const float CLOCK_SPEED;

        bool runCycle(); //run a cycle of tia, return true to break CPU syncronisation (for things like WSYNC)

        TelevisionInterfaceAdapter(Memory* memory);
    };
}

#endif