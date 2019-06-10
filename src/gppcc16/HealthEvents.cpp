#include "gppcc16/HealthEvents.hpp"

namespace gppcc16
{
    OnTriggerDamage::OnTriggerDamage() :
        OnTriggerDamage(math::AABBf(), 0, -1, nullptr)
    { }

    OnTriggerDamage::OnTriggerDamage(const math::AABBf& area, int damage, int fromteam, Component* sender) :
        area(area),
        damage(damage),
        fromteam(fromteam),
        sender(sender)
    { }

    OnShouldTakeDamage::OnShouldTakeDamage() :
        OnShouldTakeDamage(nullptr, nullptr, -1, nullptr, nullptr)
    { }

    OnShouldTakeDamage::OnShouldTakeDamage(bool* execute, int* damage, int fromteam, HealthComponent* receiver, Component* sender) :
        execute(execute),
        damage(damage),
        fromteam(fromteam),
        receiver(receiver),
        sender(sender)
    { }

    OnHealthChanged::OnHealthChanged() :
        OnHealthChanged(0, 0, -1, nullptr, nullptr)
    { }

    OnHealthChanged::OnHealthChanged(int delta, int newhp, int fromteam, HealthComponent* receiver, Component* sender) :
        delta(delta),
        newhp(newhp),
        fromteam(fromteam),
        receiver(receiver),
        sender(sender)
    { }

    OnDie::OnDie() :
        OnDie(-1, nullptr, nullptr)
    { }

    OnDie::OnDie(int fromteam, HealthComponent* receiver, Component* killer) :
        fromteam(fromteam),
        receiver(receiver),
        killer(killer)
    { }
}
