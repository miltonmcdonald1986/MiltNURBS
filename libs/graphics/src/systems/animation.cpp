#include <graphics/systems/animation.h>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>

using graphics::components::color::Color;
using graphics::components::flash::Flash;

namespace graphics::systems::animation
{

    void update_flash(entt::registry& reg, double dt)
    {
        auto view = reg.view<Color, Flash>();
        for (auto [entity, color, flash] : view.each())
        {
            flash.t += dt * flash.speed;

            float intensity = static_cast<float>((std::sin(flash.t) * 0.5) + 0.5);

            color.rgba[0] = color.base[0] * intensity;
            color.rgba[1] = color.base[1] * intensity;
            color.rgba[2] = color.base[2] * intensity;
            color.rgba[3] = color.base[3];
        }
    }

} // namespace graphics::systems::animation
