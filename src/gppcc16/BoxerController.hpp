#ifndef GPPCC16_BOXERCONTROLLER_HPP
#define GPPCC16_BOXERCONTROLLER_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/core/res/SpriteResource.hpp"
#include "gamelib/core/res/SoundResource.hpp"
#include "gamelib/components/update/QController.hpp"
#include "gamelib/core/event/Event.hpp"

namespace gppcc16
{
    using namespace gamelib;

    enum State
    {
        Stand,
        Walk,
        Punch,
        // Duck,
        Block,
        Stagger,
        KO,
        Win,
        NumStates
    };

    constexpr unsigned int input_left  = gamelib::input_left;
    constexpr unsigned int input_right = gamelib::input_right;
    constexpr unsigned int input_punch = gamelib::input_userbegin;
    constexpr unsigned int input_block = gamelib::input_userbegin << 1;
    // constexpr unsigned int input_duck  = gamelib::input_userbegin << 2;

    class BoxerController : public gamelib::UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("BoxerController");

        public:
            BoxerController();

            auto update(float elapsed) -> void final override;

            auto getInput() const             -> unsigned int;
            auto setInput(unsigned int input) -> void;

            auto getState() const -> State;

        protected:
            auto _init() -> bool final override;
            auto _quit() -> void final override;

            auto _handleInput()         -> void;
            auto _inputToState()        -> void;
            auto _onAnimationEnd()      -> void;
            auto _setState(State state) -> void;
            auto _camshake(float multiplier = 1) const -> void;

            static auto _OnDie_Handler(BoxerController* self, EventPtr event)             -> void;
            static auto OnShouldTakeDamage_Handler(BoxerController* self, EventPtr event) -> void;

        public:
            bool handleInput;
            SpriteResource::Handle sprites[NumStates];
            SoundResource::Handle punchsound;
            SoundResource::Handle blocksound;
            SoundResource::Handle walksound;
            SoundResource::Handle kosound;

            static float knockback;
            static float damage;
            static int punchEndurance;
            static float blockRegenMultiplier;
            static float blockDamageFactor;
            static float blockEndurancePenalty;
            static float hitBlockedEndurancePenalty;

        private:
            sf::Sound _sound;
            State _state;
            unsigned int _input;
            unsigned int _oldinput;
            bool _lastframe;
            bool _punched;
    };
}

#endif
