#include "gppcc16/EnduranceComponent.hpp"

namespace gppcc16
{
    EnduranceComponent::EnduranceComponent() :
        endurance(100),
        maxEndurance(100),
        regenSpeed(10),
        regenMultiplier(1),
        cooldown(1),
        emptyPenalty(3)
    {
        _props.registerProperty("endurance", endurance);
        _props.registerProperty("maxEndurance", maxEndurance);
        _props.registerProperty("regenSpeed", regenSpeed);
        _props.registerProperty("regenMultiplier", regenMultiplier);
        _props.registerProperty("emptyPenalty", emptyPenalty);
        _props.registerProperty("cooldown", cooldown);
    }

    auto EnduranceComponent::drain(float amount) -> void
    {
        endurance -= amount;
        if (endurance < 0)
            _penaltytimer.start(emptyPenalty);
        else
            _penaltytimer.start(cooldown);
        endurance = math::clamp(endurance, 0.f, maxEndurance);
    }

    auto EnduranceComponent::getFactor() const -> float
    {
        return maxEndurance == 0 ? 0 : endurance / maxEndurance;
    }

    auto EnduranceComponent::reset() -> void
    {
        endurance = maxEndurance;
        _penaltytimer.stop();
    }

    auto EnduranceComponent::update(float elapsed) -> void
    {
        if (_penaltytimer.isRunning())
        {
            _penaltytimer.update(elapsed);
            if (_penaltytimer.isFinished())
                _penaltytimer.stop();
        }
        else
            endurance = std::min(maxEndurance, endurance + regenSpeed * regenMultiplier * elapsed);
    }
}
