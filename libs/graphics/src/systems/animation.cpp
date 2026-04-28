#include <graphics/systems/animation.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/shake.h>
#include <graphics/components/transform.h>
#include <graphics/components/world_matrix.h>

using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::shake::Shake;
using graphics::components::shake::ShakeOnce;
using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;

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
        auto view = reg.view<WorldMatrix, Shake>();
        for (auto [e, wm, shake] : view.each())
        {
            shake.t += dt * shake.speed;

            float dx = static_cast<float>(std::sin(shake.t * 1.3f)) * shake.intensity;
            float dy = static_cast<float>(std::sin(shake.t * 2.1f)) * shake.intensity;
            float dz = static_cast<float>(std::sin(shake.t * 3.7f)) * shake.intensity;

            glm::vec3 offset(dx, dy, dz);
            wm.value = shake.base_world * glm::translate(glm::mat4(1), offset);
        }
    }

    void update_shake_base_world(entt::registry& reg, entt::entity e, const glm::mat4& world_matrix)
    {
        if (Shake* shake = reg.try_get<Shake>(e))
            shake->base_world = world_matrix;
        else if (ShakeOnce* shake_once = reg.try_get<ShakeOnce>(e))
            shake_once->base_world = world_matrix;
    }

    void update_shake_once(entt::registry& reg, double dt)
    {
        auto view = reg.view<WorldMatrix, ShakeOnce>();
        for (auto [e, wm, shake] : view.each())
        {
            // Countdown
            shake.time_left -= static_cast<float>(dt);
            if (shake.time_left <= 0.f)
            {
                // Restore original position
                reg.remove<ShakeOnce>(e);
                continue;
            }

            // Compute shake intensity falloff (optional)
            float alpha = shake.time_left / shake.duration;
            float current_intensity = shake.intensity * alpha;

            float dx = std::sin(shake.time_left * shake.speed * 1.3f) * current_intensity;
            float dy = std::sin(shake.time_left * shake.speed * 2.1f) * current_intensity;
            float dz = std::sin(shake.time_left * shake.speed * 3.7f) * current_intensity;

            glm::vec3 offset(dx, dy, dz);
            wm.value = shake.base_world * glm::translate(glm::mat4(1), offset);
        }
    }


} // namespace graphics::systems::animation
