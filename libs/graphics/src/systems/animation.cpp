#include <graphics/systems/animation.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/shake.h>
#include <graphics/components/transform.h>
#include <graphics/components/world_matrix.h>

namespace graphics::systems
{

    void update_flash(entt::registry& reg, float dt)
    {
        auto view = reg.view<components::Flash>();
        for (auto [e, flash] : view.each())
            flash.t += dt * flash.speed;
    }

    void update_shake(entt::registry& reg, float dt)
    {
        auto view = reg.view<components::WorldMatrix, components::Shake>();
        for (auto [e, wm, shake] : view.each())
        {
            shake.t += dt * shake.speed;

            float dx = std::sin(shake.t * 1.3f) * shake.intensity;
            float dy = std::sin(shake.t * 2.1f) * shake.intensity;
            float dz = std::sin(shake.t * 3.7f) * shake.intensity;

            glm::vec3 offset(dx, dy, dz);
            wm.value = shake.base_world * glm::translate(glm::mat4(1), offset);
        }
    }

    void update_shake_base_world(entt::registry& reg, entt::entity e, const glm::mat4& world_matrix)
    {
        if (components::Shake* shake = reg.try_get<components::Shake>(e))
            shake->base_world = world_matrix;
        else if (components::ShakeOnce* shake_once = reg.try_get<components::ShakeOnce>(e))
            shake_once->base_world = world_matrix;
    }

    void update_shake_once(entt::registry& reg, float dt)
    {
        auto view = reg.view<components::WorldMatrix, components::ShakeOnce>();
        for (auto [e, wm, shake] : view.each())
        {
            // Countdown
            shake.time_left -= dt;
            if (shake.time_left <= 0.f)
            {
                // Restore original position
                reg.remove<components::ShakeOnce>(e);
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


}
