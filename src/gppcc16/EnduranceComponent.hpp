#ifndef GPPCC16_ENDURANCE_HPP
#define GPPCC16_ENDURANCE_HPP

#include "gamelib/components/UpdateComponent.hpp"
#include "gamelib/utils/Timer.hpp"

using namespace gamelib;

namespace gppcc16
{
    class EnduranceComponent : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("Endurance");

        public:
            EnduranceComponent();
            virtual ~EnduranceComponent() {};

            auto drain(float amount) -> void;
            auto getFactor() const -> float;
            auto reset() -> void;

            virtual auto update(float elapsed) -> void override;

        public:
            float endurance;
            float maxEndurance;
            float regenSpeed;
            float regenMultiplier;
            float cooldown;
            float emptyPenalty;

        protected:
            Timer _penaltytimer;
    };
}

#endif
