#include "SFMLAdapter.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream> //TEMP

using namespace Atari2600::DisplayAdapter;

//Public Methods
void SFMLAdapter::renderPixel(Position& pos, Colour& colour)
{
    std::cout << " >>>>> RENDER PIXEL ";
    std::cout << "colour(" << (int)colour.r << "," << (int)colour.g << "," << (int)colour.b << ") ";
    std::cout << "at position (" << (int)pos.x << ',' << (int)pos.y << ") <<<<<" << std::endl;

    //TODO: update pixel array
}

void SFMLAdapter::init()
{
    _window = new sf::RenderWindow(sf::VideoMode(800, 600), "my window");
    _texture = new sf::Texture();

    _texture->create(WIDTH, HEIGHT);
}

SFMLAdapter::~SFMLAdapter()
{
    delete _window;
    delete _texture;
}