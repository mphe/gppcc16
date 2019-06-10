#ifndef GAMELIB_TARGETCOMPONENT_HPP
#define GAMELIB_TARGETCOMPONENT_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/utils/Identifier.hpp"

using namespace gamelib;

namespace gppcc16
{
    class TargetComponent : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("Target");

        public:
            TargetComponent();
            virtual ~TargetComponent() {}

            auto setTarget(const std::string& name) -> void;
            virtual auto update(float elapsed) -> void override;

        public:
            Component* applycomp;
            float transitionSpeed;

        protected:
            Entity::Handle _target;
            std::string _targetname;
    };
}

#endif
