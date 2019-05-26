#ifndef _TELEVISION_INTERFACE_ADAPTER_H_
#define _TELEVISION_INTERFACE_ADAPTER_H_

typedef unsigned char byte;

namespace Atari2600
{
    class TelevisionInterfaceAdapter
    {
        //Constants
        static const float CLOCK_SPEED_MHZ;

        unsigned short _cycles;
    };
}

#endif