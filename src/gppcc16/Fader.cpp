#include "gppcc16/Fader.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include <climits>

namespace gppcc16
{
    auto FadeUpdater::update(float elapsed) -> void
    {
        if (fader)
            fader->update(elapsed);
    }



    Fader::Fader() :
        color(sf::Color::Black),
        _timer(1)
    {
        _props.registerProperty("fadetime", _timer.seconds, +[](const float* val, Fader* self)
                {
                    self->_timer.start(*val);
                }, this);
        _props.registerProperty("color", color);
        updater.fader = this;
    }

    auto Fader::update(float elapsed) -> void
    {
        _timer.update(elapsed);
        sf::Color newcolor(color);
        newcolor.a = 255 * (1 - _timer.getPercent());

        const sf::Color colors[] = {
            newcolor, newcolor, newcolor, newcolor
        };
        _system->updateNodeMesh(_handle, 4, 0, nullptr, nullptr, colors);
    }

    auto Fader::_init() -> bool
    {
        if (!RenderComponent::_init())
            return false;

        const sf::Vector2f vertices[] = {
            sf::Vector2f(-1000, -1000),
            sf::Vector2f(1000, -1000),
            sf::Vector2f(1000, 1000),
            sf::Vector2f(-1000, 1000)
        };

        const sf::Color colors[] = {
            sf::Color::Transparent,
            sf::Color::Transparent,
            sf::Color::Transparent,
            sf::Color::Transparent,
        };

        _system->createNodeMesh(_handle, 4, sf::TriangleFan);
        _system->updateNodeMesh(_handle, 4, 0, vertices, nullptr, colors);

        return updater._init();
    }

    auto Fader::_quit() -> void
    {
        updater._quit();
        RenderComponent::_quit();
    }
}
