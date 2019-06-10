#ifndef GPPCC16_HEALTHEVENTS_HPP
#define GPPCC16_HEALTHEVENTS_HPP

#include "gamelib/core/event/Event.hpp"
#include "math/geometry/AABB.hpp"

using namespace gamelib;

namespace gamelib
{
    class Component;
}

namespace gppcc16
{
    class HealthComponent;

    class OnTriggerDamage : public Event<0xd82d4c9f, OnTriggerDamage>
    {
        public:
            OnTriggerDamage();
            OnTriggerDamage(const math::AABBf& area, int damage, int fromteam, Component* sender);

        public:
            math::AABBf area;
            int damage;
            int fromteam;
            Component* sender;
    };

    class OnShouldTakeDamage : public Event<0x133ee88c, OnShouldTakeDamage>
    {
        public:
            OnShouldTakeDamage();
            OnShouldTakeDamage(bool* execute, int* damage, int fromteam, HealthComponent* receiver, Component* sender);

        public:
            bool* execute;
            int* damage;
            int fromteam;
            HealthComponent* receiver;
            Component* sender;
    };


    class OnHealthChanged : public Event<0x954d7f9f, OnHealthChanged>
    {
        public:
            OnHealthChanged();
            OnHealthChanged(int delta, int newhp, int fromteam, HealthComponent* receiver, Component* sender);

        public:
            int delta;
            int newhp;
            int fromteam;
            HealthComponent* receiver;
            Component* sender;
    };

    class OnDie : public Event<0xe36af6dc, OnDie>
    {
        public:
            OnDie();
            OnDie(int fromteam, HealthComponent* receiver, Component* killer);

        public:
            int fromteam;
            HealthComponent* receiver;
            Component* killer;
    };
}

#endif
