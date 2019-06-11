#include "SFMLAdapter.h"
#include <SFML/Window.hpp>

using namespace Atari2600::DisplayAdapter;

//Public Methods
void SFMLAdapter::renderPixel(Position& pos, Colour& colour)
{

}

void SFMLAdapter::init()
{
    sf::Window window;
    window.create(sf::VideoMode(800, 600), "My window");

    while(window.isOpen())
    {
        
    }
}