#include "TelevisionInterfaceAdapter.h"
#include "Memory.h"
#include "DisplayAdapter/DisplayAdapterInterface.h"
#include <iostream>
#include <cmath>

using namespace Atari2600;
using namespace std;

typedef TelevisionInterfaceAdapter TIA;

//--Private:
const byte TIA::CLOCKS_PER_SCANLINE = 228;
const unsigned short TIA::MAX_SCANLINES = 262;
const byte TIA::VERTICAL_SYNC_THRESHOLD = 3;
const byte TIA::VERTICAL_PICTURE_THRESHOLD = 37 + TIA::VERTICAL_SYNC_THRESHOLD;
const byte TIA::VERTICAL_OVERSCAN_THRESHOLD = 232;
const byte TIA::HORIZONTAL_PICTURE_THRESHOLD = 68;
const byte TIA::PLAYFIELD_HALF = 19;

//register constants
const byte TIA::VSYNC = 0x00;
const byte TIA::COLUPF = 0x08;
const byte TIA::COLUBK = 0x09;
const byte TIA::CTRLPF = 0x0A;
const byte TIA::PF0 = 0x0D;
const byte TIA::PF1 = 0x0E;
const byte TIA::PF2 = 0x0F;
const byte TIA::COLUP0 = 0x06;
const byte TIA::COLUP1 = 0x07;
const byte TIA::GRP0 = 0x1B;
const byte TIA::GRP1 = 0x1C;

bool TIA::shouldRenderPlayer()
{
    return false; //TODO
}

bool TIA::shouldRenderPlayfield()
{
    /* 20 bits determine one half of the playfield
     * 4 clocks happen per bit, work out what bit we are currently on
     * The order of bits are:
     * PF0: 4 ~ 7 (0 ~ 3)
     * PF1: 7 ~ 0 (4 ~ 11)
     * PF2: 0 ~ 7 (12 ~ 19)
     */
    int currentBit = (int)((_clockCounter - HORIZONTAL_PICTURE_THRESHOLD) / 4);
    bool reflection = (_memory->read(CTRLPF) & 1) == 1;

    //muck around with the currentBit value to determine reflection/duplication
    if(currentBit > PLAYFIELD_HALF)
        if(reflection) //Reflection: decrement currentBit back to 0
            currentBit = PLAYFIELD_HALF - (currentBit - PLAYFIELD_HALF - 1);
        else //Duplication: Just reset the bit back to 0 after 20 and re render the first 20
            currentBit -= PLAYFIELD_HALF + 1;

    bool renderPlayfield = false;

    //Now check if the current playfield bit is true
    if(currentBit <= PLAYFIELD_HALF - 16) //pf0
    {
        byte pf0 = _memory->read(PF0);
        int bitValue = pow(2, currentBit + 4);

        renderPlayfield = (pf0 & bitValue) == bitValue;
    }
    else if(currentBit <= PLAYFIELD_HALF - 8)
    {
        byte pf1 = _memory->read(PF1);
        int bitValue = pow(2, currentBit + (3 - ((currentBit - 4) * 2)));

        renderPlayfield = (pf1 & bitValue) == bitValue;
    }
    else if(currentBit <= PLAYFIELD_HALF)
    {
        byte pf2 = _memory->read(PF2);
        int bitValue = pow(2, currentBit - 12);

        renderPlayfield = (pf2 & bitValue) == bitValue;
    }

    return renderPlayfield;
}

DisplayAdapter::Colour TIA::resolveColour(byte value)
{
    DisplayAdapter::Colour colour;

    switch(value)
    {
        case 0x02:
        case 0x03:
            colour.r = colour.g = colour.b = 0x1A;
            break;
        case 0x04:
        case 0x05:
            colour.r = colour.g = colour.b = 0x39;
            break;
        case 0x06:
        case 0x07:
            colour.r = colour.g = colour.b = 0x5B;
            break;
        case 0x08:
        case 0x09:
            colour.r = colour.g = colour.b = 0x7E;
            break;
        case 0x0A:
        case 0x0B:
            colour.r = colour.g = colour.b = 0xA2;
            break;
        case 0x0C:
        case 0x0D:
            colour.r = colour.g = colour.b = 0xC7;
            break;
        case 0x0E:
        case 0x0F:
            colour.r = colour.g = colour.b = 0xED;
            break;

        case 0x10:
        case 0x11:
            colour.r = 0x19; colour.g = 0x02; colour.b = 0x00;
            break;
        case 0x12:
        case 0x13:
            colour.r = 0x3A; colour.g = 0x1F; colour.b = 0x00;
            break;
        case 0x14:
        case 0x15:
            colour.r = 0x5D; colour.g = 0x41; colour.b = 0x00;
            break;
        case 0x16:
        case 0x17:
            colour.r = 0x82; colour.g = 0x64; colour.b = 0x00;
            break;
        case 0x18:
        case 0x19:
            colour.r = 0xA7; colour.g = 0x88; colour.b = 0x00;
            break;
        case 0x1A:
        case 0x1B:
            colour.r = 0xCC; colour.g = 0xAD; colour.b = 0x00;
            break;
        case 0x1C:
        case 0x1D:
            colour.r = 0xF2; colour.g = 0xD2; colour.b = 0x19;
            break;
        case 0x1E:
        case 0x1F:
            colour.r = 0xFE; colour.g = 0xFA; colour.b = 0x40;
            break;

        case 0x20:
        case 0x21:
            colour.r = 0x37; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0x22:
        case 0x23:
            colour.r = 0x5E; colour.g = 0x08; colour.b = 0x00;
            break;
        case 0x24:
        case 0x25:
            colour.r = 0x83; colour.g = 0x27; colour.b = 0x00;
            break;
        case 0x26:
        case 0x27:
            colour.r = 0xA9; colour.g = 0x49; colour.b = 0x00;
            break;
        case 0x28:
        case 0x29:
            colour.r = 0xCF; colour.g = 0x6C; colour.b = 0x00;
            break;
        case 0x2A:
        case 0x2B:
            colour.r = 0xF5; colour.g = 0x8F; colour.b = 0x17;
            break;
        case 0x2C:
        case 0x2D:
            colour.r = 0xFE; colour.g = 0xB4; colour.b = 0x38;
            break;
        case 0x2E:
        case 0x2F:
            colour.r = 0xFE; colour.g = 0xDF; colour.b = 0x6F;
            break;

        case 0x30:
        case 0x31:
            colour.r = 0x47; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0x32:
        case 0x33:
            colour.r = 0x73; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0x34:
        case 0x35:
            colour.r = 0x98; colour.g = 0x13; colour.b = 0x00;
            break;
        case 0x36:
        case 0x37:
            colour.r = 0xBE; colour.g = 0x32; colour.b = 0x16;
            break;
        case 0x38:
        case 0x39:
            colour.r = 0xE4; colour.g = 0x53; colour.b = 0x35;
            break;
        case 0x3A:
        case 0x3B:
            colour.r = 0xFE; colour.g = 0x76; colour.b = 0x57;
            break;
        case 0x3C:
        case 0x3D:
            colour.r = 0xFE; colour.g = 0x9C; colour.b = 0x81;
            break;
        case 0x3E:
        case 0x3F:
            colour.r = 0xFE; colour.g = 0xC6; colour.b = 0xBB;
            break;

        case 0x40:
        case 0x41:
            colour.r = 0x44; colour.g = 0x00; colour.b = 0x08;
            break;
        case 0x42:
        case 0x43:
            colour.r = 0x6F; colour.g = 0x00; colour.b = 0x1F;
            break;
        case 0x44:
        case 0x45:
            colour.r = 0x96; colour.g = 0x06; colour.b = 0x40;
            break;
        case 0x46:
        case 0x47:
            colour.r = 0xBB; colour.g = 0x24; colour.b = 0x62;
            break;
        case 0x48:
        case 0x49:
            colour.r = 0xE1; colour.g = 0x45; colour.b = 0x85;
            break;
        case 0x4A:
        case 0x4B:
            colour.r = 0xFE; colour.g = 0x67; colour.b = 0xAA;
            break;
        case 0x4C:
        case 0x4D:
            colour.r = 0xFE; colour.g = 0x8C; colour.b = 0xD6;
            break;
        case 0x4E:
        case 0x4F:
            colour.r = 0xFE; colour.g = 0xB7; colour.b = 0xF6;
            break;

        case 0x50:
        case 0x51:
            colour.r = 0x2D; colour.g = 0x00; colour.b = 0x4A;
            break;
        case 0x52:
        case 0x53:
            colour.r = 0x57; colour.g = 0x00; colour.b = 0x67;
            break;
        case 0x54:
        case 0x55:
            colour.r = 0x7D; colour.g = 0x05; colour.b = 0x8C;
            break;
        case 0x56:
        case 0x57:
            colour.r = 0xA1; colour.g = 0x22; colour.b = 0xB1;
            break;
        case 0x58:
        case 0x59:
            colour.r = 0xC7; colour.g = 0x43; colour.b = 0xD7;
            break;
        case 0x5A:
        case 0x5B:
            colour.r = 0xED; colour.g = 0x65; colour.b = 0xFE;
            break;
        case 0x5C:
        case 0x5D:
            colour.r = 0xFE; colour.g = 0x8A; colour.b = 0xF6;
            break;
        case 0x5E:
        case 0x5F:
            colour.r = 0xFE; colour.g = 0xB5; colour.b = 0xF7;
            break;

        case 0x60:
        case 0x61:
            colour.r = 0x0D; colour.g = 0x00; colour.b = 0x82;
            break;
        case 0x62:
        case 0x63:
            colour.r = 0x33; colour.g = 0x00; colour.b = 0xA2;
            break;
        case 0x64:
        case 0x65:
            colour.r = 0x55; colour.g = 0x0F; colour.b = 0xC9;
            break;
        case 0x66:
        case 0x67:
            colour.r = 0x78; colour.g = 0x2D; colour.b = 0xF0;
            break;
        case 0x68:
        case 0x69:
            colour.r = 0x9C; colour.g = 0x4E; colour.b = 0xFE;
            break;
        case 0x6A:
        case 0x6B:
            colour.r = 0xC3; colour.g = 0x72; colour.b = 0xFE;
            break;
        case 0x6C:
        case 0x6D:
            colour.r = 0xEB; colour.g = 0x98; colour.b = 0xFE;
            break;
        case 0x6E:
        case 0x6F:
            colour.r = 0xFE; colour.g = 0xC0; colour.b = 0xF9;
            break;
        
        case 0x70:
        case 0x71:
            colour.r = 0x00; colour.g = 0x00; colour.b = 0x91;
            break;
        case 0x72:
        case 0x73:
            colour.r = 0x0A; colour.g = 0x05; colour.b = 0xBD;
            break;
        case 0x74:
        case 0x75:
            colour.r = 0x28; colour.g = 0x22; colour.b = 0xE4;
            break;
        case 0x76:
        case 0x77:
            colour.r = 0x48; colour.g = 0x42; colour.b = 0xFE;
            break;
        case 0x78:
        case 0x79:
            colour.r = 0x6B; colour.g = 0x64; colour.b = 0xFE;
            break;
        case 0x7A:
        case 0x7B:
            colour.r = 0x90; colour.g = 0x8A; colour.b = 0xFE;
            break;
        case 0x7C:
        case 0x7D:
            colour.r = 0xB7; colour.g = 0xB0; colour.b = 0xFE;
            break;
        case 0x7E:
        case 0x7F:
            colour.r = 0xDF; colour.g = 0xD8; colour.b = 0xFE;
            break;

        case 0x80:
        case 0x81:
            colour.r = 0x00; colour.g = 0x00; colour.b = 0x72;
            break;
        case 0x82:
        case 0x83:
            colour.r = 0x00; colour.g = 0x1C; colour.b = 0xAB;
            break;
        case 0x84:
        case 0x85:
            colour.r = 0x03; colour.g = 0x3C; colour.b = 0xD6;
            break;
        case 0x86:
        case 0x87:
            colour.r = 0x20; colour.g = 0x5E; colour.b = 0xFD;
            break;
        case 0x88:
        case 0x89:
            colour.r = 0x40; colour.g = 0x81; colour.b = 0xFE;
            break;
        case 0x8A:
        case 0x8B:
            colour.r = 0x64; colour.g = 0xA6; colour.b = 0xFE;
            break;
        case 0x8C:
        case 0x8D:
            colour.r = 0x89; colour.g = 0xCE; colour.b = 0xFE;
            break;
        case 0x8E:
        case 0x8F:
            colour.r = 0xB0; colour.g = 0xF6; colour.b = 0xFE;
            break;

        case 0x90:
        case 0x91:
            colour.r = 0x00; colour.g = 0x10; colour.b = 0x3A;
            break;
        case 0x92:
        case 0x93:
            colour.r = 0x00; colour.g = 0x31; colour.b = 0x6E;
            break;
        case 0x94:
        case 0x95:
            colour.r = 0x00; colour.g = 0x55; colour.b = 0xA2;
            break;
        case 0x96:
        case 0x97:
            colour.r = 0x05; colour.g = 0x79; colour.b = 0xC8;
            break;
        case 0x98:
        case 0x99:
            colour.r = 0x23; colour.g = 0x9D; colour.b = 0xEE;
            break;
        case 0x9A:
        case 0x9B:
            colour.r = 0x44; colour.g = 0xC2; colour.b = 0xFE;
            break;
        case 0x9C:
        case 0x9D:
            colour.r = 0x68; colour.g = 0xE9; colour.b = 0xFE;
            break;
        case 0x9E:
        case 0x9F:
            colour.r = 0x8F; colour.g = 0xFE; colour.b = 0xFE;
            break;

        case 0xA0:
        case 0xA1:
            colour.r = 0x00; colour.g = 0x1F; colour.b = 0x02;
            break;
        case 0xA2:
        case 0xA3:
            colour.r = 0x00; colour.g = 0x43; colour.b = 0x26;
            break;
        case 0xA4:
        case 0xA5:
            colour.r = 0x00; colour.g = 0x69; colour.b = 0x57;
            break;
        case 0xA6:
        case 0xA7:
            colour.r = 0x00; colour.g = 0x8D; colour.b = 0x7A;
            break;
        case 0xA8:
        case 0xA9:
            colour.r = 0x1B; colour.g = 0xB1; colour.b = 0x9E;
            break;
        case 0xAA:
        case 0xAB:
            colour.r = 0x3B; colour.g = 0xD7; colour.b = 0xC3;
            break;
        case 0xAC:
        case 0xAD:
            colour.r = 0x5D; colour.g = 0xFE; colour.b = 0xE9;
            break;
        case 0xAE:
        case 0xAF:
            colour.r = 0x86; colour.g = 0xFE; colour.b = 0xFE;
            break;

        case 0xB0:
        case 0xB1:
            colour.r = 0x00; colour.g = 0x24; colour.b = 0x03;
            break;
        case 0xB2:
        case 0xB3:
            colour.r = 0x00; colour.g = 0x4A; colour.b = 0x05;
            break;
        case 0xB4:
        case 0xB5:
            colour.r = 0x00; colour.g = 0x70; colour.b = 0x0C;
            break;
        case 0xB6:
        case 0xB7:
            colour.r = 0x09; colour.g = 0x95; colour.b = 0x2B;
            break;
        case 0xB8:
        case 0xB9:
            colour.r = 0x28; colour.g = 0xBA; colour.b = 0x4C;
            break;
        case 0xBA:
        case 0xBB:
            colour.r = 0x49; colour.g = 0xE0; colour.b = 0x6E;
            break;
        case 0xBC:
        case 0xBD:
            colour.r = 0x6C; colour.g = 0xFE; colour.b = 0x92;
            break;
        case 0xBE:
        case 0xBF:
            colour.r = 0x97; colour.g = 0xFE; colour.b = 0xB5;
            break;

        case 0xC0:
        case 0xC1:
            colour.r = 0x00; colour.g = 0x21; colour.b = 0x02;
            break;
        case 0xC2:
        case 0xC3:
            colour.r = 0x00; colour.g = 0x46; colour.b = 0x04;
            break;
        case 0xC4:
        case 0xC5:
            colour.r = 0x08; colour.g = 0x6B; colour.b = 0x00;
            break;
        case 0xC6:
        case 0xC7:
            colour.r = 0x28; colour.g = 0x90; colour.b = 0x00;
            break;
        case 0xC8:
        case 0xC9:
            colour.r = 0x49; colour.g = 0xB5; colour.b = 0x09;
            break;
        case 0xCA:
        case 0xCB:
            colour.r = 0x6B; colour.g = 0xDB; colour.b = 0x28;
            break;
        case 0xCC:
        case 0xCD:
            colour.r = 0x8F; colour.g = 0xFE; colour.b = 0x49;
            break;
        case 0xCE:
        case 0xCF:
            colour.r = 0xBB; colour.g = 0xFE; colour.b = 0x69;
            break;

        case 0xD0:
        case 0xD1:
            colour.r = 0x00; colour.g = 0x15; colour.b = 0x01;
            break;
        case 0xD2:
        case 0xD3:
            colour.r = 0x10; colour.g = 0x36; colour.b = 0x00;
            break;
        case 0xD4:
        case 0xD5:
            colour.r = 0x30; colour.g = 0x59; colour.b = 0x00;
            break;
        case 0xD6:
        case 0xD7:
            colour.r = 0x53; colour.g = 0x7E; colour.b = 0x00;
            break;
        case 0xD8:
        case 0xD9:
            colour.r = 0x76; colour.g = 0xA3; colour.b = 0x00;
            break;
        case 0xDA:
        case 0xDB:
            colour.r = 0x9A; colour.g = 0xC8; colour.b = 0x00;
            break;
        case 0xDC:
        case 0xDD:
            colour.r = 0xBF; colour.g = 0xEE; colour.b = 0x1E;
            break;
        case 0xDE:
        case 0xDF:
            colour.r = 0xE8; colour.g = 0xFE; colour.b = 0x3E;
            break;

        case 0xE0:
        case 0xE1:
            colour.r = 0x1A; colour.g = 0x02; colour.b = 0x00;
            break;
        case 0xE2:
        case 0xE3:
            colour.r = 0x3B; colour.g = 0x1F; colour.b = 0x00;
            break;
        case 0xE4:
        case 0xE5:
            colour.r = 0x5E; colour.g = 0x41; colour.b = 0x00;
            break;
        case 0xE6:
        case 0xE7:
            colour.r = 0x83; colour.g = 0x64; colour.b = 0x00;
            break;
        case 0xE8:
        case 0xE9:
            colour.r = 0xA8; colour.g = 0x88; colour.b = 0x00;
            break;
        case 0xEA:
        case 0xEB:
            colour.r = 0xCE; colour.g = 0xAD; colour.b = 0x00;
            break;
        case 0xEC:
        case 0xED:
            colour.r = 0xF4; colour.g = 0xD2; colour.b = 0x18;
            break;
        case 0xEE:
        case 0xEF:
            colour.r = 0xFE; colour.g = 0xFA; colour.b = 0x40;
            break;
        
        case 0xF0:
        case 0xF1:
            colour.r = 0x38; colour.g = 0x00; colour.b = 0x00;
            break;
        case 0xF2:
        case 0xF3:
            colour.r = 0x5F; colour.g = 0x08; colour.b = 0x00;
            break;
        case 0xF4:
        case 0xF5:
            colour.r = 0x84; colour.g = 0x27; colour.b = 0x00;
            break;
        case 0xF6:
        case 0xF7:
            colour.r = 0xAA; colour.g = 0x49; colour.b = 0x00;
            break;
        case 0xF8:
        case 0xF9:
            colour.r = 0xD0; colour.g = 0x6B; colour.b = 0x00;
            break;
        case 0xFA:
        case 0xFB:
            colour.r = 0xF6; colour.g = 0x8F; colour.b = 0x18;
            break;
        case 0xFC:
        case 0xFD:
            colour.r = 0xFE; colour.g = 0xB4; colour.b = 0x39;
            break;
        case 0xFE:
        case 0xFF:
            colour.r = 0xFE; colour.g = 0xDF; colour.b = 0x70;
            break;
        
        default:
            colour.r = colour.g = colour.b = 0;
    }

    return colour;
}

DisplayAdapter::Colour TIA::determinePixel(DisplayAdapter::Position pos)
{
    if(shouldRenderPlayfield())
    {
        bool usePlayerColours = (_memory->read(CTRLPF) & 2) == 2;

        if(usePlayerColours)
        {
            bool isFirstHalf = ((_clockCounter - HORIZONTAL_PICTURE_THRESHOLD) / 4) <= PLAYFIELD_HALF;

            if(isFirstHalf) //use player 1 colour
                return resolveColour(_memory->read(COLUP0));
            else //use player 2 colour
                return resolveColour(_memory->read(COLUP1));
        }
        
        return resolveColour(_memory->read(COLUPF));
    }

    //default to background colour
    return resolveColour(_memory->read(COLUBK));
}

void TIA::handleVSYNC()
{
    byte vsyncTrigger = _memory->read(VSYNC);

    //vsync has been triggered by the program and the TIA isn't in vsync already
    if(vsyncTrigger != 0 && !_vsync)
    {
        _clockCounter = 0;
        _vScanlineCounter = 0;
        _vsync = true;
    }

    if(_vScanlineCounter > VERTICAL_SYNC_THRESHOLD && _vsync)
        _vsync = false;
}

void TIA::renderScanline()
{
    //Ready to draw visible lines
    if(_vScanlineCounter >= VERTICAL_PICTURE_THRESHOLD &&
        _vScanlineCounter < VERTICAL_OVERSCAN_THRESHOLD &&
        _clockCounter >= HORIZONTAL_PICTURE_THRESHOLD)
    {
        DisplayAdapter::Position pos;
        pos.x = _clockCounter - HORIZONTAL_PICTURE_THRESHOLD;
        pos.y = _vScanlineCounter - VERTICAL_PICTURE_THRESHOLD;

        //determine pixel colour
        auto colour = determinePixel(pos);

        _displayAdapter->renderPixel(pos, colour);
    }
    
    if(++_clockCounter == CLOCKS_PER_SCANLINE)
    {
        _clockCounter = 0;

        //TODO: not sure what is involved when a full frame has been rendered
        if(++_vScanlineCounter == MAX_SCANLINES)
            _vScanlineCounter = 0;
    }
}

//--Public:
const float TIA::CLOCK_SPEED = 3.58; //mhz

TIAState TIA::runCycle()
{
    if(!_displayAdapter->isRunning())
        return TIAState::DISPLAY_EXITED;

    handleVSYNC();
    
    if(_memory->isWSYNC()) //handle WSYNC
    {
        byte remainingCycles = CLOCKS_PER_SCANLINE - _clockCounter;
        while(remainingCycles-- > 0)
            renderScanline();

        return TIAState::WSYNC;
    }
    else
        renderScanline();

    return TIAState::OK;
}

TIA::TelevisionInterfaceAdapter(DisplayAdapter::DisplayAdapterInterface* displayAdapter, Memory* memory)
    : _displayAdapter(displayAdapter), _memory(memory),
      _clockCounter(0), _vScanlineCounter(0),
      _vsync(false)
{
    _displayAdapter->init();
}

TIA::~TelevisionInterfaceAdapter()
{
    delete _displayAdapter;
}