#ifndef GPPCC16_BOXERAI_HPP
#define GPPCC16_BOXERAI_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/update/Updatable.hpp"
#include "gamelib/utils/Timer.hpp"
#include <memory>

using namespace gamelib;

namespace gppcc16
{
    class BoxerAI;
    class BoxerController;
    class EnduranceComponent;
    class HealthComponent;

    struct AIContext
    {
        BoxerAI* ai;
        BoxerController* controller;
        HealthComponent* hp;
        EnduranceComponent* endurance;
        Entity::Handle player;
        Entity* playerptr;
        BoxerController* playercontroller;
        Entity* self;

        AIContext();
    };

    class AIPolicy : public Updatable
    {
        public:
            AIPolicy(AIContext& context);
            virtual ~AIPolicy() {};

            virtual auto isFinished()      -> bool = 0;

        protected:
            AIContext* _context;
    };

    class BoxerAI : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("BoxerAI")

        public:
            BoxerAI();

            auto update(float elapsed) -> void override;
            auto inReach() const -> bool;

        private:
            auto _refresh(RefreshType type, Component* comp) -> void final override;

        private:
            Timer _timer;
            AIContext _context;
            std::unique_ptr<AIPolicy> _policy;
    };
}

#endif
