#include "SFMLAdapter.h"

#include <iostream> //TEMP

using namespace Atari2600::DisplayAdapter;

//Public Methods
void SFMLAdapter::renderPixel(Position& pos, Colour& colour)
{
    std::cout << " >>>>> RENDER PIXEL <<<<< " << std::endl;
}

void SFMLAdapter::init()
{
    std::cout << " >>>>> DISPLAY INIT <<<<< " << std::endl;
}