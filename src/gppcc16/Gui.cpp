#include "gppcc16/Gui.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "HealthComponent.hpp"
#include "EnduranceComponent.hpp"
#include "HealthEvents.hpp"
#include <SFML/Graphics.hpp>

namespace gppcc16
{
    void OnDie_Handler(GuiState* self, EventPtr event)
    {
        self->disable = true;
    }

    GuiState::GuiState() :
        disable(false)
    { }

    auto GuiState::init(Game* game) -> bool
    {
        registerEvent<OnDie>(OnDie_Handler, this);
        return true;
    }

    auto GuiState::quit() -> void
    {
        unregisterEvent<OnDie>(OnDie_Handler, this);
    }


    auto GuiState::update(float elapsed) -> void
    {
    }

    auto GuiState::render(sf::RenderTarget& target) -> void
    {
        if (disable)
            return;

        constexpr const char* names[] = { "player", "enemy" };
        static Entity::Handle handles[2];
        const math::Vec2f size(0.32, 0.075);
        const math::Vec2f offsets[2] = {
            math::Vec2f(0.02, 0.03),
            math::Vec2f(1 - (size.x + offsets[0].x), offsets[0].y)
        };

        for (size_t i = 0; i < 2; ++i)
        {
            if (!handles[i])
                handles[i] = findEntityHandle(names[i]);

            if (!handles[i])
                continue;

            auto ent = getEntity(handles[i]);

            for (size_t i = 0; i < 2; ++i)
                ent->findByName<HealthComponent>();

            HealthComponent* health = ent->findByName<HealthComponent>();
            if (!health)
                continue;

            _drawHealthbar(target, offsets[i], size, health->hp, health->maxhp);

            EnduranceComponent* endurance = ent->findByName<EnduranceComponent>();
            if (!endurance)
                continue;

            const math::Vec2f endusize(0.2, 0.06);
            _drawHealthbar(target, offsets[i] + math::Vec2f(i * (size.x - endusize.x), size.y + 0.01), endusize, endurance->endurance, endurance->maxEndurance);
        }
    }

    auto GuiState::_drawHealthbar(sf::RenderTarget& target, const math::Vec2f& pospercent, const math::Vec2f& sizepercent, int value, int max) -> void
    {
        constexpr int padding = 2;

        auto camsys = CameraSystem::getActive();
        auto cam = camsys->get(0);
        auto camrect = cam->getCamRect();
        cam->apply(target);

        auto pos = camrect.pos + camrect.size * pospercent;
        auto size = camrect.size * sizepercent;
        // pos.x = std::round(pos.x);
        // pos.y = std::round(pos.y);
        // size.x = std::round(size.x);
        // size.y = std::round(size.y);

        sf::RectangleShape barbg;
        barbg.setFillColor(sf::Color::Black);
        barbg.setSize(convert(size));
        barbg.setPosition(convert(pos));
        target.draw(barbg);

        if (value <= 0)
            return;

        sf::RectangleShape bar;
        bar.setFillColor(sf::Color::White);
        bar.setPosition(pos.x + padding, pos.y + padding);
        bar.setSize(sf::Vector2f((size.x - 2 * padding) * value / (float)max, size.y - 2 * padding));
        target.draw(bar);
    }
}
