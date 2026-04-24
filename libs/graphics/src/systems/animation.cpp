#include <graphics/systems/animation.h>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/shake.h>
#include <graphics/components/transform.h>

using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::shake::Shake;
using graphics::components::shake::ShakeOnce;
using graphics::components::transform::Transform;

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

    void update_shake(entt::registry& reg, double dt)
    {
        auto view = reg.view<Transform, Shake>();
        for (auto [e, tform, shake] : view.each())
        {
            shake.t += dt * shake.speed;

            float dx = static_cast<float>(std::sin(shake.t * 1.3f)) * shake.intensity;
            float dy = static_cast<float>(std::sin(shake.t * 2.1f)) * shake.intensity;
            float dz = static_cast<float>(std::sin(shake.t * 3.7f)) * shake.intensity;

            // Always shake around the original base position
            tform.position = shake.base_position + glm::vec3(dx, dy, dz);
        }
    }

    void update_shake_base_position(entt::registry& reg, entt::entity e, const glm::vec3& position)
    {
        if (Shake* shake = reg.try_get<Shake>(e))
        {
            shake->base_position = position;
        }
    }

    void update_shake_once(entt::registry& reg, double dt)
    {
        auto view = reg.view<Transform, ShakeOnce>();
        for (auto [e, tform, shake] : view.each())
        {
            // Countdown
            shake.time_left -= static_cast<float>(dt);
            if (shake.time_left <= 0.f)
            {
                // Restore original position
                tform.position = shake.base_position;
                reg.remove<ShakeOnce>(e);
                continue;
            }

            // Compute shake intensity falloff (optional)
            float alpha = shake.time_left / shake.duration;
            float current_intensity = shake.intensity * alpha;

            shake.base_position = shake.base_position; // unchanged

            float dx = std::sin(shake.time_left * shake.speed * 1.3f) * current_intensity;
            float dy = std::sin(shake.time_left * shake.speed * 2.1f) * current_intensity;
            float dz = std::sin(shake.time_left * shake.speed * 3.7f) * current_intensity;

            tform.position = shake.base_position + glm::vec3(dx, dy, dz);
        }
    }


} // namespace graphics::systems::animation
