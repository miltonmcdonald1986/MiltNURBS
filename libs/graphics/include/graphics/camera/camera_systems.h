#ifndef GRAPHICS_CAMERA_CAMERA_SYSTEMS_H
#define GRAPHICS_CAMERA_CAMERA_SYSTEMS_H

#include <entt/entt.hpp>

#include <glm/glm.hpp>

namespace graphics::camera
{

    glm::mat4 compute_projection(const entt::registry& reg, entt::entity cam_ent, float aspect);
    void update_camera_system(entt::registry& reg, float dt);

}

#endif // GRAPHICS_CAMERA_CAMERA_SYSTEMS_H