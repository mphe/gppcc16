#include "gppcc16/BoxerAI.hpp"
#include "HealthComponent.hpp"
#include "EnduranceComponent.hpp"
#include "BoxerController.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"

namespace gppcc16
{
    AIContext::AIContext() :
        ai(nullptr),
        controller(nullptr),
        hp(nullptr),
        endurance(nullptr),
        playerptr(nullptr),
        playercontroller(nullptr),
        self(nullptr)
    { }


    AIPolicy::AIPolicy(AIContext& context) :
        _context(&context)
    { }


    class BlockPolicy : public AIPolicy
    {
        public:
            BlockPolicy(AIContext& context) :
                AIPolicy(context)
            { }

            auto update(float elapsed) -> void final override
            {
                _context->controller->setInput(input_block);
            }

            auto isFinished() -> bool final override
            {
                return false;
            }
    };

    class WalkPolicy : public AIPolicy
    {
        public:
            WalkPolicy(AIContext& context, bool away) :
                AIPolicy(context),
                _away(away)
            { }

            auto update(float elapsed) -> void final override
            {
                auto hdist = getDistance();
                int dir = (_away ? -1 : 1) * math::sign(hdist);

                unsigned int input = dir < 0 ? input_left : input_right;
                _context->controller->setInput(input);
            }

            auto getDistance() const -> int
            {
                return (_context->playerptr->getTransform().getPosition() - _context->self->getTransform().getPosition()).x;
            }

            auto isFinished() -> bool final override
            {
                return _away ^ _context->ai->inReach();
            }

        private:
            bool _away;
    };

    class PunchPolicy : public AIPolicy
    {
        public:
            PunchPolicy(AIContext& context) :
                AIPolicy(context),
                _finished(false),
                _punched(false)
            { }

            auto update(float elapsed) -> void final override
            {
                if (_finished)
                    return;

                if (_context->controller->getState() != Punch)
                {
                    if (_punched)
                    {
                        _finished = true;
                        return;
                    }

                    _context->controller->setInput(input_punch);
                    _punched = true;
                }
            }

            auto isFinished() -> bool final override
            {
                return _finished;
            }

        private:
            bool _finished;
            bool _punched;
    };


    BoxerAI::BoxerAI() :
        UpdateComponent(1, UpdateHookType::PostFrame),
        _timer(1)
    {
        _props.registerProperty("thinktime", _timer.seconds);
        _context.ai = this;
    }

    auto BoxerAI::update(float elapsed) -> void
    {
        if (!_context.player)
            _context.player = findEntityHandle("player");

        if (_context.player)
        {
            _context.playerptr = ::getEntity(_context.player);
            _context.playercontroller = _context.playerptr->findByName<BoxerController>();

            if (_context.playercontroller->getState() == KO)
            {
                _timer.stop();
                _policy.reset();
                return;
            }
        }

        _timer.update(elapsed);

        if (_policy)
        {
            _policy->update(elapsed);
            if (_policy->isFinished())
                _timer.stop();
        }

        if (!_policy || !_timer.isRunning())
        {
            bool inreach = inReach();
            _policy.reset();

            if (inreach)
            {
                if (_context.playercontroller->getState() == Punch && rand() % 100 <= 33)
                {
                    if (_context.endurance->getFactor() < 0.2)
                        _policy.reset(new WalkPolicy(_context, true));
                    else
                        _policy.reset(new BlockPolicy(_context));
                }
                else
                {
                    if (_context.endurance->getFactor() < 0.2)
                        _policy.reset(new WalkPolicy(_context, true));
                    else
                    {
                        switch (rand() % 6)
                        {
                            case 0:
                                _policy.reset(new WalkPolicy(_context, false));
                                break;
                            case 1:
                                _policy.reset(new WalkPolicy(_context, true));
                                break;
                            case 2:
                                break;
                            case 3:
                            case 4:
                                _policy.reset(new PunchPolicy(_context));
                                break;
                            case 5:
                                _policy.reset(new BlockPolicy(_context));
                                break;
                        }
                    }
                }
            }

            if (!inreach)
            {
                if (_context.endurance->getFactor() > 0.5 && rand() % 3 == 0)
                    _policy.reset(new WalkPolicy(_context, false));
                else
                    switch (rand() % 3)
                    {
                        case 0:
                            _policy.reset(new WalkPolicy(_context, false));
                            break;
                        case 1:
                            _policy.reset(new WalkPolicy(_context, true));
                            break;
                        case 2:
                            break;
                    }
            }

            _timer.start();
        }
    }

    auto BoxerAI::inReach() const -> bool
    {
        auto colsys = CollisionSystem::getActive();
        auto hitbox = getEntity()->findByName<AABB>();
        return colsys->intersect(getEntity()->getTransform().getBBox(), hitbox, collision_hitbox);
    }

    auto BoxerAI::_refresh(RefreshType type, Component* comp) -> void
    {
        auto ent = getEntity();
        _context.self = ent;
        _context.controller = ent->findByName<BoxerController>();
        _context.hp         = ent->findByName<HealthComponent>();
        _context.endurance  = ent->findByName<EnduranceComponent>();
    }
}
