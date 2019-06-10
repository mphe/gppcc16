#ifndef GPPCC16_GUI_HPP
#define GPPCC16_GUI_HPP

#include "gamelib/core/GameState.hpp"
#include "math/geometry/Vector.hpp"

using namespace gamelib;

namespace gamelib
{
    class Entity;
}

namespace gppcc16
{
    class HealthComponent;

    class GuiState : public GameState
    {
        public:
            GuiState();

            auto init(Game* game) -> bool final override;
            auto quit()           -> void final override;

            auto update(float elapsed)            -> void final override;
            auto render(sf::RenderTarget& target) -> void final override;

        public:
            bool disable;

        private:
            auto _drawHealthbar(sf::RenderTarget& target, const math::Vec2f& pospercent, const math::Vec2f& sizepercent, int value, int max) -> void;
    };
}

#endif
