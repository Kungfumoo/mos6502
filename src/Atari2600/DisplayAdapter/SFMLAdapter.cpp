#include "SFMLAdapter.h"
#include <limits>

#include <iostream> //TEMP

using namespace Atari2600::DisplayAdapter;
using namespace std;

//--Private
const unsigned int SFMLAdapter::PIXEL_LIMIT = SFMLAdapter::WIDTH * SFMLAdapter::HEIGHT * 4;
const string SFMLAdapter::FONT_FILE = "resources/fonts/FreeSans.ttf";

void SFMLAdapter::renderFps()
{
    auto end = chrono::system_clock::now();
    chrono::duration<double> timeTaken = end - _startFrame;
    double timeInSeconds = _frames / timeTaken.count();

    //TODO: is it ok to create a text object at the end of every frame?
    sf::Text text(to_string((int)timeInSeconds), _debugFont);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
    text.setFillColor(sf::Color::Green);

    _window.draw(text);

    _startFrame = end;
    _frames = 0;
}

void SFMLAdapter::renderWindow()
{
    while(_window.isOpen())
    {
        if(_renderQueue.empty())
            continue;

        _renderQueueMutex.lock();
        PixelVector pixels = _renderQueue.top();
        _renderQueue.pop();
        _renderQueueMutex.unlock();

        sf::Sprite sprite(_texture);
        sprite.setScale((float)_windowWidth / WIDTH, (float)_windowHeight / HEIGHT); //scale to window size

        _texture.update(pixels.data());
        _window.clear();
        _window.draw(sprite);
        renderFps();
        _window.display();

        _frames++;
    }
}

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
        _renderQueueMutex.lock();
        _renderQueue.push(_pixels);
        _renderQueueMutex.unlock();
    }
}

void SFMLAdapter::init()
{
    _window.create(sf::VideoMode(_windowWidth, _windowHeight), "my window");
    _window.setActive(false);

    _startFrame = chrono::system_clock::now();
    _renderThread = std::thread(&SFMLAdapter::renderWindow, this);
}

SFMLAdapter::SFMLAdapter(unsigned int windowWidth, unsigned int windowHeight)
    : _windowWidth(windowWidth), _windowHeight(windowHeight), _window(),
      _texture(), _pixels(PIXEL_LIMIT, 0),
      _debugFont(), _frames(0), _renderThread(),
      _renderQueue(), _renderQueueMutex(),
      _startFrame()
{
    _texture.create(WIDTH, HEIGHT);
    _debugFont.loadFromFile(FONT_FILE);
}

SFMLAdapter::~SFMLAdapter()
{
    if(_renderThread.joinable())
        _renderThread.join();
}