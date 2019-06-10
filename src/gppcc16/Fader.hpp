#ifndef GPPCC16_FADER_HPP
#define GPPCC16_FADER_HPP

#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/utils/Timer.hpp"

using namespace gamelib;

namespace gppcc16
{
    class Fader;

    class FadeUpdater : public UpdateComponent
    {
        friend class Fader;

        public:
            ASSIGN_NAMETAG("FadeUpdater");

            auto update(float elapsed) -> void final override;

        public:
            Fader* fader;
    };

    class Fader : public RenderComponent, public Updatable
    {
        public:
            ASSIGN_NAMETAG("Fader");

            Fader();
            auto update(float elapsed) -> void final override;

        private:
            auto _init() -> bool final override;
            auto _quit() -> void final override;

        public:
            sf::Color color;

        private:
            FadeUpdater updater;
            Timer _timer;
    };
}

#endif
