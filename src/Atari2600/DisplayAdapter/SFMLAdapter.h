#ifndef _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_
#define _ATARI_2600_DISPLAY_ADAPTER_SFML_ADAPTER_H_

#include "DisplayAdapterInterface.h"

//forward dec
namespace sf
{
    class RenderWindow;
    class Texture;
}

namespace Atari2600
{
namespace DisplayAdapter
{
    class SFMLAdapter : public DisplayAdapterInterface
    {
        sf::RenderWindow* _window;
        sf::Texture* _texture;

    public:
        void renderPixel(Position& pos, Colour& colour) override;
        void init() override;

        ~SFMLAdapter() override;
    };
}
}

#endif