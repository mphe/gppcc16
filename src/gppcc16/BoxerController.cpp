#include "gppcc16/BoxerController.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "HealthComponent.hpp"
#include "HealthEvents.hpp"
#include "EnduranceComponent.hpp"
#include "GameEvents.hpp"
// #include "TargetComponent.hpp"
#include "gamelib/properties/PropResource.hpp"
#include "gamelib/events/CameraEvents.hpp"

namespace gppcc16
{
    void BoxerController::_OnDie_Handler(BoxerController* self, EventPtr event)
    {
        auto ev = event->get<OnDie>();
        if (ev->receiver->getEntity() == self->getEntity())
            self->_setState(KO);
        else
            self->_setState(Win);
    }

    void BoxerController::OnShouldTakeDamage_Handler(BoxerController* self, EventPtr event)
    {
        auto ev = event->get<OnShouldTakeDamage>();

        if (ev->sender == self)
            return;

        auto ent = self->getEntity();
        auto other = ev->sender->getEntity();
        auto endurance = ent->findByName<EnduranceComponent>();

        float knockbackMultiplier = 1;
        float shakeMultiplier = 1;
        bool blocked = false;

        if (self->getState() == Block)
            if (endurance && endurance->endurance > 0)
                blocked = true;

        if (blocked)
        {
            knockbackMultiplier = 0.25;
            shakeMultiplier = 0.25;

            // apply endurance drain
            endurance->drain(BoxerController::blockEndurancePenalty);
            other->findByName<EnduranceComponent>()->drain(BoxerController::hitBlockedEndurancePenalty);

            // absorb damage
            *ev->damage = *ev->damage * BoxerController::blockDamageFactor;

            self->_sound.setBuffer(*self->blocksound);
            self->_sound.play();
        }
        else
        {
            self->_setState(Stagger);
            self->_sound.setBuffer(*self->punchsound);
            self->_sound.play();
        }

        self->_camshake(shakeMultiplier);

        const auto knockbackdir = ent->getTransform().getPosition() - other->getTransform().getPosition();
        const auto knockbackvel = knockbackdir.normalized() * BoxerController::knockback * knockbackMultiplier;
        ent->findByName<QPhysics>()->vel += knockbackvel;
        if (blocked)
            other->findByName<QPhysics>()->vel -= knockbackvel;
    }

    float BoxerController::knockback = 200;
    float BoxerController::damage = 10;
    int BoxerController::punchEndurance = 20;
    float BoxerController::blockRegenMultiplier = 0.5;
    float BoxerController::blockDamageFactor = 0;
    float BoxerController::blockEndurancePenalty = punchEndurance;
    float BoxerController::hitBlockedEndurancePenalty = 0;

    BoxerController::BoxerController() :
        handleInput(true),
        _state(Stand),
        _lastframe(false),
        _punched(false)
    {
#define REGISTER_STATE_SPR(state) registerResourceProperty(_props, #state, sprites[state]);

        _props.registerProperty("handleInput", handleInput);
        _props.registerProperty("knockback", knockback);
        _props.registerProperty("punchEndurance", punchEndurance);
        _props.registerProperty("blockRegenMultiplier", blockRegenMultiplier);
        _props.registerProperty("blockDamageFactor", blockDamageFactor);
        _props.registerProperty("blockEndurancePenalty", blockEndurancePenalty);
        _props.registerProperty("hitBlockedEndurancePenalty", hitBlockedEndurancePenalty);
        registerResourceProperty(_props, "punchsound", punchsound);
        registerResourceProperty(_props, "blocksound", blocksound);
        registerResourceProperty(_props, "walksound", walksound);
        registerResourceProperty(_props, "kosound", kosound);
        REGISTER_STATE_SPR(Stand);
        REGISTER_STATE_SPR(Walk);
        REGISTER_STATE_SPR(Block);
        // REGISTER_STATE_SPR(Duck);
        REGISTER_STATE_SPR(Punch);
        REGISTER_STATE_SPR(Stagger);
        REGISTER_STATE_SPR(KO);
        REGISTER_STATE_SPR(Win);
    }

    auto BoxerController::update(float elapsed) -> void
    {
        if (handleInput)
            _handleInput();

        _inputToState();

        if (_state == Walk)
        {
            auto qcontrol = getEntity()->findByName<QController>();
            if (qcontrol)
                qcontrol->setInput(_input);
        }

        auto sprite = getEntity()->findByName<SpriteComponent>();
        auto& ani = sprite->getAnimation();
        int anioffset = ani.ani.offset;

        { // Check for animation end
            if (anioffset == ani.ani.length - 1)
            {
                _lastframe = true;

                if ((_state == KO || _state == Win) && ani.ani.speed != 0)
                {
                    // auto ent = getEntity();
                    // auto target = ent->findByName<TargetComponent>();
                    // if (target)
                    //     ent->remove(target);
                    //
                    // if (ent->getTransform().getPosition())

                    sprite->setIndex(ani.ani.length - 1);
                    sprite->setSpeed(0);

                    if (_state == KO)
                    {
                        triggerEvent<OnKO>(getEntity()->getHandle());
                        _sound.setBuffer(*kosound);
                        _sound.play();
                        _camshake(1.5);
                    }
                }
            }

            if (_lastframe && anioffset == 0)
            {
                _lastframe = false;
                _onAnimationEnd();
            }
        }

        if (_state == Punch)
        {
            if (anioffset == 3 && !_punched)
            {
                auto health = getEntity()->findByName<HealthComponent>();
                _punched = true;
                queueEvent<OnTriggerDamage>(getEntity()->getTransform().getBBox(), damage, health->team, this);
            }
        }
        else if (_state == Walk)
        {
            if (walksound && anioffset == 3)
            {
                _sound.setBuffer(*walksound);
                _sound.play();
            }
        }

        _oldinput = _input;
        _input = 0;
    }

    auto BoxerController::getInput() const -> unsigned int
    {
        return _input;
    }

    auto BoxerController::setInput(unsigned int input) -> void
    {
        _input = input;
    }

    auto BoxerController::getState() const -> State
    {
        return _state;
    }


    auto BoxerController::_handleInput() -> void
    {
        auto input = InputSystem::getActive();
        unsigned buttons = 0;

        if (input->isKeyPressed(sf::Keyboard::E))
            buttons |= input_punch;
        else if (input->isKeyDown(sf::Keyboard::Space))
            buttons |= input_block;
        // else if (input->isKeyDown(sf::Keyboard::S))
        //     buttons |= input_duck;
        else
        {
            if (input->isKeyDown(sf::Keyboard::A))
                buttons |= input_left;
            if (input->isKeyDown(sf::Keyboard::D))
                buttons |= input_right;
        }

        setInput(buttons);
    }

    auto BoxerController::_inputToState() -> void
    {
        if (_state == KO || _state == Win || _state == Stagger || _state == Punch)
            return;

        State newstate = Stand;

        if (_input & input_punch)
            newstate = Punch;
        else if (_input & input_block)
            newstate = Block;
        // else if (_input & input_duck)
        //     newstate = Duck;
        else if (_input & input_left || _input & input_right)
            newstate = Walk;

        if (_state == newstate)
            return;

        // Check and update endurance
        auto endurance = getEntity()->findByName<EnduranceComponent>();
        if (endurance)
        {
            if (newstate == Punch)
            {
                if (endurance->endurance <= 0)
                    return;
                else
                    endurance->drain(punchEndurance);
            }

            // Blocking slows down endurance regeneration
            endurance->regenMultiplier = newstate == Block ? blockRegenMultiplier : 1;
        }

        _setState(newstate);
    }

    auto BoxerController::_setState(State state) -> void
    {
        if (_state == state)
            return;
        _state = state;
        _punched = false;
        _lastframe = false;
        getEntity()->findByName<SpriteComponent>()->change(sprites[_state]);
    }

    auto BoxerController::_onAnimationEnd() -> void
    {
        // auto sprite = getEntity()->findByName<SpriteComponent>();
        _lastframe = false;

        if (_state == Punch)
        {
            _punched = false;
            _setState(Stand);
        }
        else if (_state == Stagger)
        {
            _setState(Stand);
        }
    }

    auto BoxerController::_camshake(float multiplier) const -> void
    {
        triggerEvent<TriggerCameraShake>(multiplier);
    }

    auto BoxerController::_init() -> bool
    {
        if (!UpdateComponent::_init())
            return false;
        auto evmgr = EventManager::getActive();
        evmgr->regCallback<OnShouldTakeDamage>(OnShouldTakeDamage_Handler, this);
        evmgr->regCallback<OnDie>(_OnDie_Handler, this);
        return true;
    }

    auto BoxerController::_quit() -> void
    {
        auto evmgr = EventManager::getActive();
        evmgr->unregCallback<OnShouldTakeDamage>(OnShouldTakeDamage_Handler, this);
        evmgr->unregCallback<OnDie>(_OnDie_Handler, this);
        UpdateComponent::_quit();
    }
}
