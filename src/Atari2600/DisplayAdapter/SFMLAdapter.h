#ifndef _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_
#define _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_

#include "DisplayAdapterInterface.h"
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <string>
#include <thread>
#include <stack>
#include <mutex>
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> SystemClock;
typedef std::vector<sf::Uint8> PixelVector;

namespace Atari2600
{
namespace DisplayAdapter
{
    class SFMLAdapter : public DisplayAdapterInterface
    {
        static const unsigned int PIXEL_LIMIT;
        static const std::string FONT_FILE;

        unsigned int _windowWidth;
        unsigned int _windowHeight;
        sf::RenderWindow _window;
        sf::Texture _texture;
        PixelVector _pixels;
        sf::Font _debugFont;
        unsigned int _frames;
        std::thread _renderThread;
        std::stack<PixelVector> _renderQueue;
        std::mutex _renderQueueMutex;
        SystemClock _startFrame;

        void renderFps();
        void renderWindow();

    public:
        void renderPixel(Position& pos, Colour& colour) override;
        void init() override;

        SFMLAdapter(unsigned int windowWidth, unsigned int windowHeight);
        ~SFMLAdapter() override;
    };
}
}

#endif