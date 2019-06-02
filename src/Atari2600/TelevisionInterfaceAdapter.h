#ifndef _AT2600_TELEVISION_INTERFACE_ADAPTER_H_
#define _AT2600_TELEVISION_INTERFACE_ADAPTER_H_

typedef unsigned char byte;

namespace Atari2600
{
    //forward dec
    class Memory;

    class TelevisionInterfaceAdapter
    {
        Memory* _memory;

    public:
        static const float CLOCK_SPEED;

        void runCycle();

        TelevisionInterfaceAdapter(Memory* memory);
    };
}

#endif