#ifndef GPPCC16_HEALTHCOMPONENT_HPP
#define GPPCC16_HEALTHCOMPONENT_HPP

#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/utils/Identifier.hpp"

using namespace gamelib;

namespace gppcc16
{
    class HealthComponent : public Identifier<0x5dd848e7, Component>
    {
        public:
            ASSIGN_NAMETAG("Health");

        public:
            HealthComponent();
            virtual ~HealthComponent();

            auto dealDamage(int damage) -> void;
            auto dealDamage(int damage, int fromteam, Component* sender) -> void;

            auto reset() -> void;

        protected:
            virtual auto _init() -> bool override;
            virtual auto _quit() -> void override;

        public:
            int maxhp;
            int hp;
            int team;
    };
}

#endif
