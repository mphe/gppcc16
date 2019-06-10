#include "TargetComponent.hpp"
#include "gamelib/properties/PropComponent.hpp"

namespace gppcc16
{
    TargetComponent::TargetComponent() :
        applycomp(nullptr),
        transitionSpeed(2)
    {
        _props.registerProperty("transitionSpeed", transitionSpeed);
        _props.registerProperty("target", _targetname,
                +[](const std::string* val, TargetComponent* self)
                {
                    self->setTarget(*val);
                }, this);
        registerProperty(_props, "component", applycomp, *this);
    }

    auto TargetComponent::setTarget(const std::string& name) -> void
    {
        _target.reset();
        _targetname = name;
    }

    auto TargetComponent::update(float elapsed) -> void
    {
        if (!_target)
        {
            auto ent = findEntity(_targetname);
            if (ent)
                _target = ent->getHandle();
        }

        auto ent = gamelib::getEntity(_target);

        if (!ent || !_target || !applycomp || !applycomp->getTransform())
            return;

        auto transform = applycomp->getTransform();
        auto scale = transform->getScale();
        auto dirsigns = (ent->getTransform().getPosition() - transform->getPosition()).signs();

        if (transitionSpeed == 0)
        {
            for (size_t i = 0; i < dirsigns.size(); ++i)
                if (dirsigns[i] == 0)
                    dirsigns[i] = math::sign(scale[i]);
            transform->setScale(math::abs(scale) * dirsigns);
        }
        else
        {
            for (size_t i = 0; i < dirsigns.size(); ++i)
            {
                // TODO: make work for scalings other than 1/-1
                if (dirsigns[i] < 0)
                    scale[i] = std::fmax(-1, scale[i] - transitionSpeed * elapsed);
                else if (dirsigns.x > 0)
                    scale[i] = std::fmin(1, scale[i] + transitionSpeed * elapsed);
                else
                    scale[i] = scale[i] < 0 ? -1 : 1;
            }

            transform->setScale(scale);
        }
    }
}
