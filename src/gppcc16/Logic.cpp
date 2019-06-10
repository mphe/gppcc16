#include "gppcc16/Logic.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/res/SoundResource.hpp"
#include "GameEvents.hpp"
#include "HealthEvents.hpp"
#include "HealthComponent.hpp"
#include "gamelib/events/CameraEvents.hpp"

namespace gppcc16
{
    void Logic::_OnDie_Handler(Logic* self, EventPtr event)
    {
        auto ev = event->get<OnDie>();
        if (ev->receiver->getEntity()->getName() == "player")
            createEntity("fader");
        else
        {
            createEntity("winfader");
            auto enemy = findEntity("enemy");
            enemy->findByName<SpriteComponent>()->setDepth(-5);
            self->_win = true;
        }
        self->_endscreenTimer.start();
    }



    Logic::Logic() :
        _endscreenTimer(3),
        _win(false)
    {
        _props.registerProperty("textdelay", _endscreenTimer.seconds);
    }

    auto Logic::update(float elapsed) -> void
    {
        if (_endscreenTimer.isRunning())
            _endscreenTimer.update(elapsed);

        if (_endscreenTimer)
        {
            _endscreenTimer.stop();
            auto rensys = RenderSystem::getActive();
            auto layer = rensys->findLayer(_win ? "win" : "gameover");
            auto flags = toggleflag(rensys->getLayer(layer)->options.flags, render_hidden);
            rensys->setLayerOptions(layer, &flags);

            triggerEvent<TriggerCameraShake>();

            auto sound = ResourceManager::getActive()->get("sound/ko.wav").as<SoundResource>();
            if (sound)
            {
                _sound.setBuffer(*sound);
                _sound.play();
            }
        }
    }

    auto Logic::_init() -> bool
    {
        if (!UpdateComponent::_init())
            return false;

        registerEvent<OnDie>(_OnDie_Handler, this);
        return true;
    }

    auto Logic::_quit() -> void
    {
        unregisterEvent<OnDie>(_OnDie_Handler, this);
    }
}
