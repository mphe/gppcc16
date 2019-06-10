#ifndef GPPCC16_LOGIC_HPP
#define GPPCC16_LOGIC_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/utils/Timer.hpp"
#include "gamelib/core/event/Event.hpp"
#include <SFML/Audio.hpp>

using namespace gamelib;

namespace gppcc16
{
    class Logic : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("Logic");

            Logic();

            auto update(float elapsed) -> void final override;

        protected:
            auto _init() -> bool final override;
            auto _quit() -> void final override;

            static auto _OnDie_Handler(Logic* self, EventPtr event) -> void;

        private:
            Timer _endscreenTimer;
            sf::Sound _sound;
            bool _win;
    };
}

#endif
