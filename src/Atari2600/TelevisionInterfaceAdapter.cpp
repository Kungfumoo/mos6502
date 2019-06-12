#include "TelevisionInterfaceAdapter.h"
#include "Memory.h"
#include "DisplayAdapter/DisplayAdapterInterface.h"
#include <iostream>

using namespace Atari2600;
using namespace std;

typedef TelevisionInterfaceAdapter TIA;

//--Private:
const byte TIA::CLOCKS_PER_SCANLINE = 228;
const unsigned short TIA::MAX_SCANLINES = 262;
const byte TIA::VERTICAL_PICTURE_THRESHOLD = 40;
const byte TIA::HORIZONTAL_PICTURE_THRESHOLD = 68;

DisplayAdapter::Colour TIA::resolveColour(byte value)
{
    DisplayAdapter::Colour colour;

    switch(value)
    {
        case 0x02:
            colour.r = colour.g = colour.b = 0x1A;
            break;
        case 0x04:
            colour.r = colour.g = colour.b = 0x39;
            break;
        case 0x06:
            colour.r = colour.g = colour.b = 0x5B;
            break;
        case 0x08:
            colour.r = colour.g = colour.b = 0x7E;
            break;
        case 0x0A:
            colour.r = colour.g = colour.b = 0xA2;
            break;
        case 0x0C:
            colour.r = colour.g = colour.b = 0xC7;
            break;
        case 0x0E:
            colour.r = colour.g = colour.b = 0xED;
            break;

        case 0x10:
            colour.r = 0x19; colour.g = 0x02; colour.b = 0x00;
            break;
        case 0x12:
            colour.r = 0x3A; colour.g = 0x1F; colour.b = 0x00;
            break;
        case 0x14:
            colour.r = 0x5D; colour.g = 0x41; colour.b = 0x00;
            break;
        case 0x16:
            colour.r = 0x82; colour.g = 0x64; colour.b = 0x00;
            break;
        case 0x18:
            colour.r = 0xA7; colour.g = 0x88; colour.b = 0x00;
            break;
        case 0x1A:
            colour.r = 0xCC; colour.g = 0xAD; colour.b = 0x00;
            break;
        case 0x1C:
            colour.r = 0xF2; colour.g = 0xD2; colour.b = 0x19;
            break;
        case 0x1E:
            colour.r = 0xFE; colour.g = 0xFA; colour.b = 0x40;
            break;

        case 0x20:
            colour.r = 0x37; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0x22:
            colour.r = 0x5E; colour.g = 0x08; colour.b = 0x00;
            break;
        case 0x24:
            colour.r = 0x83; colour.g = 0x27; colour.b = 0x00;
            break;
        case 0x26:
            colour.r = 0xA9; colour.g = 0x49; colour.b = 0x00;
            break;
        case 0x28:
            colour.r = 0xCF; colour.g = 0x6C; colour.b = 0x00;
            break;
        case 0x2A:
            colour.r = 0xF5; colour.g = 0x8F; colour.b = 0x17;
            break;
        case 0x2C:
            colour.r = 0xFE; colour.g = 0xB4; colour.b = 0x38;
            break;
        case 0x2E:
            colour.r = 0xFE; colour.g = 0xDF; colour.b = 0x6F;
            break;

        case 0x30:
            colour.r = 0x47; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0x32:
            colour.r = 0x73; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0x34:
            colour.r = 0x98; colour.g = 0x13; colour.b = 0x00;
            break;
        case 0x36:
            colour.r = 0xBE; colour.g = 0x32; colour.b = 0x16;
            break;
        case 0x38:
            colour.r = 0xE4; colour.g = 0x53; colour.b = 0x35;
            break;
        case 0x3A:
            colour.r = 0xFE; colour.g = 0x76; colour.b = 0x57;
            break;
        case 0x3C:
            colour.r = 0xFE; colour.g = 0x9C; colour.b = 0x81;
            break;
        case 0x3E:
            colour.r = 0xFE; colour.g = 0xC6; colour.b = 0xBB;
            break;

        case 0x40:
            colour.r = 0x44; colour.g = 0x00; colour.b = 0x08;
            break;
        case 0x42:
            colour.r = 0x6F; colour.g = 0x00; colour.b = 0x1F;
            break;
        case 0x44:
            colour.r = 0x96; colour.g = 0x06; colour.b = 0x40;
            break;
        case 0x46:
            colour.r = 0xBB; colour.g = 0x24; colour.b = 0x62;
            break;
        case 0x48:
            colour.r = 0xE1; colour.g = 0x45; colour.b = 0x85;
            break;
        case 0x4A:
            colour.r = 0xFE; colour.g = 0x67; colour.b = 0xAA;
            break;
        case 0x4C:
            colour.r = 0xFE; colour.g = 0x8C; colour.b = 0xD6;
            break;
        case 0x4E:
            colour.r = 0xFE; colour.g = 0xB7; colour.b = 0xF6;
            break;

        case 0x50:
            colour.r = 0x2D; colour.g = 0x00; colour.b = 0x4A;
            break;
        case 0x52:
            colour.r = 0x57; colour.g = 0x00; colour.b = 0x67;
            break;
        case 0x54:
            colour.r = 0x7D; colour.g = 0x05; colour.b = 0x8C;
            break;
        case 0x56:
            colour.r = 0xA1; colour.g = 0x22; colour.b = 0xB1;
            break;
        case 0x58:
            colour.r = 0xC7; colour.g = 0x43; colour.b = 0xD7;
            break;
        case 0x5A:
            colour.r = 0xED; colour.g = 0x65; colour.b = 0xFE;
            break;
        case 0x5C:
            colour.r = 0xFE; colour.g = 0x8A; colour.b = 0xF6;
            break;
        case 0x5E:
            colour.r = 0xFE; colour.g = 0xB5; colour.b = 0xF7;
            break;

        case 0x60:
            colour.r = 0x0D; colour.g = 0x00; colour.b = 0x82;
            break;
        case 0x62:
            colour.r = 0x33; colour.g = 0x00; colour.b = 0xA2;
            break;
        case 0x64:
            colour.r = 0x55; colour.g = 0x0F; colour.b = 0xC9;
            break;
        case 0x66:
            colour.r = 0x78; colour.g = 0x2D; colour.b = 0xF0;
            break;
        case 0x68:
            colour.r = 0x9C; colour.g = 0x4E; colour.b = 0xFE;
            break;
        case 0x6A:
            colour.r = 0xC3; colour.g = 0x72; colour.b = 0xFE;
            break;
        case 0x6C:
            colour.r = 0xEB; colour.g = 0x98; colour.b = 0xFE;
            break;
        case 0x6E:
            colour.r = 0xFE; colour.g = 0xC0; colour.b = 0xF9;
            break;
        
        case 0x70:
            colour.r = 0x00; colour.g = 0x00; colour.b = 0x91;
            break;
        case 0x72:
            colour.r = 0x0A; colour.g = 0x05; colour.b = 0xBD;
            break;
        case 0x74:
            colour.r = 0x28; colour.g = 0x22; colour.b = 0xE4;
            break;
        case 0x76:
            colour.r = 0x48; colour.g = 0x42; colour.b = 0xFE;
            break;
        case 0x78:
            colour.r = 0x6B; colour.g = 0x64; colour.b = 0xFE;
            break;
        case 0x7A:
            colour.r = 0x90; colour.g = 0x8A; colour.b = 0xFE;
            break;
        case 0x7C:
            colour.r = 0xB7; colour.g = 0xB0; colour.b = 0xFE;
            break;
        case 0x7E:
            colour.r = 0xDF; colour.g = 0xD8; colour.b = 0xFE;
            break;

        case 0x80:
            colour.r = 0x00; colour.g = 0x00; colour.b = 0x72;
            break;
        case 0x82:
            colour.r = 0x00; colour.g = 0x1C; colour.b = 0xAB;
            break;
        case 0x84:
            colour.r = 0x03; colour.g = 0x3C; colour.b = 0xD6;
            break;
        case 0x86:
            colour.r = 0x20; colour.g = 0x5E; colour.b = 0xFD;
            break;
        case 0x88:
            colour.r = 0x40; colour.g = 0x81; colour.b = 0xFE;
            break;
        case 0x8A:
            colour.r = 0x64; colour.g = 0xA6; colour.b = 0xFE;
            break;
        case 0x8C:
            colour.r = 0x89; colour.g = 0xCE; colour.b = 0xFE;
            break;
        case 0x8E:
            colour.r = 0xB0; colour.g = 0xF6; colour.b = 0xFE;
            break;

        case 0x90:
            colour.r = 0x00; colour.g = 0x10; colour.b = 0x3A;
            break;
        case 0x92:
            colour.r = 0x00; colour.g = 0x31; colour.b = 0x6E;
            break;
        case 0x94:
            colour.r = 0x00; colour.g = 0x55; colour.b = 0xA2;
            break;
        case 0x96:
            colour.r = 0x05; colour.g = 0x79; colour.b = 0xC8;
            break;
        case 0x98:
            colour.r = 0x23; colour.g = 0x9D; colour.b = 0xEE;
            break;
        case 0x9A:
            colour.r = 0x44; colour.g = 0xC2; colour.b = 0xFE;
            break;
        case 0x9C:
            colour.r = 0x68; colour.g = 0xE9; colour.b = 0xFE;
            break;
        case 0x9E:
            colour.r = 0x8F; colour.g = 0xFE; colour.b = 0xFE;
            break;

        default:
            colour.r = colour.g = colour.b = 0;
    }

    return colour;
}

//--Public:
const float TIA::CLOCK_SPEED = 3.58; //mhz

bool TIA::runCycle()
{
    if(_memory->isWSYNC()) //handle WSYNC
    {
        cout << "TIA: WSYNC" << endl;

        byte remainingCycles = CLOCKS_PER_SCANLINE - _clockCounter;
        while(remainingCycles-- > 0)
            runCycle();

        return true;
    }
    else
    {
        cout << "TIA: cycle!" << endl;

        //Ready to draw visible lines
        if(_vScanlineCounter > VERTICAL_PICTURE_THRESHOLD &&
           _clockCounter > HORIZONTAL_PICTURE_THRESHOLD)
        {
            //render pixel TOOD:
        }
        
        if(_clockCounter-- == 0)
        {
            _clockCounter = CLOCKS_PER_SCANLINE;

            //TODO: not sure what is involved when a full frame has been rendered
            if(_vScanlineCounter++ > MAX_SCANLINES)
                _vScanlineCounter = 0;
        }

        return false;
    }
}

TIA::TelevisionInterfaceAdapter(DisplayAdapter::DisplayAdapterInterface* displayAdapter, Memory* memory)
{
    _clockCounter = CLOCKS_PER_SCANLINE;
    _vScanlineCounter = 0;
    _memory = memory;
    _displayAdapter = displayAdapter;

    _displayAdapter->init();
}

TIA::~TelevisionInterfaceAdapter()
{
    delete _displayAdapter;
}