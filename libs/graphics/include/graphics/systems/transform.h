#ifndef GRAPHICS_SYSTEMS_TRANSFORM_H
#define GRAPHICS_SYSTEMS_TRANSFORM_H

#include <glm/glm.hpp>

#include <entt/entt.hpp>

#include <graphics/components/transform.h>

namespace graphics::systems
{

    glm::mat4 compute_model_matrix(const components::Transform& t);
    void update_transform_dependents(entt::registry& reg);
    void update_transform_system(entt::registry& reg);
    void update_world_recursive(entt::registry& reg, entt::entity e, const glm::mat4& parentWorld, bool parent_was_dirty, const std::unordered_map<entt::entity, std::vector<entt::entity>>& children);

}

#endif // GRAPHICS_SYSTEMS_TRANSFORM_H
