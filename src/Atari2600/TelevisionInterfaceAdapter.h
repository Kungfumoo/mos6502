#ifndef _AT2600_TELEVISION_INTERFACE_ADAPTER_H_
#define _AT2600_TELEVISION_INTERFACE_ADAPTER_H_

typedef unsigned char byte;

namespace Atari2600
{
    //forward decs
    namespace DisplayAdapter
    {
        struct Colour;
        struct Position;
        class DisplayAdapterInterface;
    }

    class Memory;
    //end forward decs

    enum TIAState
    {
        OK,
        WSYNC,
        DISPLAY_EXITED
    };

    enum RenderState
    {
        PLAYER_1,
        PLAYER_2,
        BACKGROUND
    };

    class TelevisionInterfaceAdapter
    {
        static const byte CLOCKS_PER_SCANLINE;
        static const unsigned short MAX_SCANLINES;
        static const byte VERTICAL_SYNC_THRESHOLD;
        static const byte VERTICAL_PICTURE_THRESHOLD;
        static const byte VERTICAL_OVERSCAN_THRESHOLD;
        static const byte HORIZONTAL_PICTURE_THRESHOLD;
        static const byte PLAYFIELD_HALF;
        static const byte PLAYER_MAX_BITS;

        //register constants
        static const byte VSYNC;
        static const byte COLUPF;
        static const byte COLUBK;
        static const byte CTRLPF;
        static const byte PF0;
        static const byte PF1;
        static const byte PF2;
        static const byte COLUP0;
        static const byte COLUP1;
        static const byte GRP0;
        static const byte GRP1;

        DisplayAdapter::DisplayAdapterInterface* _displayAdapter;
        Memory* _memory;
        unsigned short _clockCounter;
        unsigned short _vScanlineCounter;
        bool _vsync;
        RenderState _renderState;
        byte _renderCounter;

        bool shouldRenderPlayer();
        bool shouldRenderPlayfield();
        DisplayAdapter::Colour resolveColour(byte value); //Converts the register colour value into an RBG colour (Based on NTSC)
        DisplayAdapter::Colour determinePixel(DisplayAdapter::Position pos);
        void handleVSYNC();
        void handleRenderState();
        void renderScanline();

    public:
        static const float CLOCK_SPEED;

        TIAState runCycle(); //run a cycle of tia, return state to break CPU syncronisation (for things like WSYNC)

        TelevisionInterfaceAdapter(DisplayAdapter::DisplayAdapterInterface* displayAdapter, Memory* memory);
        ~TelevisionInterfaceAdapter();
    };
}

#endif