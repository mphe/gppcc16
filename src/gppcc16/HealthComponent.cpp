#include "gppcc16/HealthComponent.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "HealthEvents.hpp"

namespace gppcc16
{
    void OnTriggerDamage_Handler(HealthComponent* self, EventPtr event)
    {
        auto ev = event->get<OnTriggerDamage>();
        if (ev->fromteam == self->team)
            return;

        self->getEntity()->findAllByType<CollisionComponent>(
                [&](CollisionComponent* comp)
                {
                    if (comp->flags & collision_hitbox && comp->intersect(ev->area))
                    {
                        self->dealDamage(ev->damage, ev->fromteam, ev->sender);
                        return true;
                    }
                    return false;
                });
    }

    HealthComponent::HealthComponent() :
        maxhp(100),
        hp(100),
        team(0)
    {
        _props.registerProperty("maxhp", maxhp);
        _props.registerProperty("hp", hp);
        _props.registerProperty("team", team);
    }

    HealthComponent::~HealthComponent()
    {
        _quit();
    }

    auto HealthComponent::reset() -> void
    {
        auto diff = maxhp - hp;
        hp = maxhp;
        triggerEvent<OnHealthChanged>(diff, hp, -1, this, nullptr);
    }

    auto HealthComponent::dealDamage(int damage) -> void
    {
        dealDamage(damage, -1, nullptr);
    }

    auto HealthComponent::dealDamage(int damage, int fromteam, Component* sender) -> void
    {
        if (team == fromteam)
            return;

        bool execute = true;
        triggerEvent<OnShouldTakeDamage>(&execute, &damage, fromteam, this, sender);

        if (!execute || damage == 0)
            return;

        hp -= damage;
        triggerEvent<OnHealthChanged>(damage, hp, fromteam, this, sender);

        if (hp <= 0)
            triggerEvent<OnDie>(fromteam, this, sender);
    }


    auto HealthComponent::_init() -> bool
    {
        auto evmgr = EventManager::getActive();
        if (evmgr)
            evmgr->regCallback<OnTriggerDamage>(OnTriggerDamage_Handler, this);

        return Component::_init();
    }

    auto HealthComponent::_quit() -> void
    {
        auto evmgr = EventManager::getActive();
        if (evmgr)
            evmgr->unregCallback<OnTriggerDamage>(OnTriggerDamage_Handler, this);

        Component::_quit();
    }
}
