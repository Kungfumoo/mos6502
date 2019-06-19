#include "SFMLAdapter.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <limits>

#include <iostream> //TEMP

using namespace Atari2600::DisplayAdapter;
using namespace std;

//--Private
const unsigned int SFMLAdapter::PIXEL_LIMIT = SFMLAdapter::WIDTH * SFMLAdapter::HEIGHT * 4;

//--Public
void SFMLAdapter::renderPixel(Position& pos, Colour& colour)
{
    /*
    std::cout << " >>>>> RENDER PIXEL ";
    std::cout << "colour(" << (int)colour.r << "," << (int)colour.g << "," << (int)colour.b << ") ";
    std::cout << "at position (" << (int)pos.x << ',' << (int)pos.y << ") <<<<<" << std::endl;
    */

    //update pixel array
    int i = (pos.x + (WIDTH * pos.y)) * 4;

    _pixels[i] = colour.r;
    _pixels[i+1] = colour.g;
    _pixels[i+2] = colour.b;
    _pixels[i+3] = numeric_limits<sf::Uint8>::max(); //alpha

    //TODO: should drawing be here?
    if(i == PIXEL_LIMIT - 4)
    {
        sf::Sprite sprite(*_texture);
        
        sprite.setScale(800.0f / WIDTH, 600.0f / HEIGHT); //scale to window size
        _texture->update(_pixels.data());
        _window->clear();
        _window->draw(sprite);
        _window->display();
    }
}

void SFMLAdapter::init()
{
    _window->create(sf::VideoMode(800, 600), "my window");
}

SFMLAdapter::SFMLAdapter()
{
    _window = new sf::RenderWindow();
    _pixels = vector<sf::Uint8>(PIXEL_LIMIT, 0);

    _texture = new sf::Texture();
    _texture->create(WIDTH, HEIGHT);
}

SFMLAdapter::~SFMLAdapter()
{
    delete _window;
    delete _texture;
}