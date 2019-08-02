#ifndef _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_
#define _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_

#include "DisplayAdapterInterface.h"
#include <SFML/Config.hpp>
#include <vector>
#include <string>

//forward dec
namespace sf
{
    class RenderWindow;
    class Texture;
    class Sprite;
    class Font;
}

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
        sf::RenderWindow* _window;
        sf::Texture* _texture;
        std::vector<sf::Uint8> _pixels;
        sf::Font* _debugFont;
        unsigned int _frames;

        void renderFps();

    public:
        void renderPixel(Position& pos, Colour& colour) override;
        void init() override;

        SFMLAdapter(unsigned int windowWidth, unsigned int windowHeight);
        ~SFMLAdapter() override;
    };
}
}

#endif