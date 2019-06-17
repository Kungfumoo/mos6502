#include "SFMLAdapter.h"
#include <SFML/Window.hpp>

#include <iostream> //TEMP

using namespace Atari2600::DisplayAdapter;

//Public Methods
void SFMLAdapter::renderPixel(Position& pos, Colour& colour)
{
    std::cout << " >>>>> RENDER PIXEL ";
    std::cout << "colour(" << (int)colour.r << "," << (int)colour.g << "," << (int)colour.b << ") ";
    std::cout << "at position (" << (int)pos.x << ',' << (int)pos.y << ") <<<<<" << std::endl;
}

void SFMLAdapter::init()
{
    _window = new sf::Window(sf::VideoMode(800, 600), "my window");
}

SFMLAdapter::~SFMLAdapter()
{
    delete _window;
}