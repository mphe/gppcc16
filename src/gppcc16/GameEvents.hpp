#ifndef GPPCC16_GAMEEVENTS_HPP
#define GPPCC16_GAMEEVENTS_HPP

#include "gamelib/core/event/Event.hpp"
#include "gamelib/core/ecs/Entity.hpp"

using namespace gamelib;

namespace gppcc16
{
    class TriggerReset : public Event<0xc5e57e7c, TriggerReset> { };

    class OnKO : public Event<0x16ddb942, OnKO>
    {
        public:
            OnKO() = default;
            OnKO(Entity::Handle ent);

        public:
            Entity::Handle ent;
    };
}

#endif
