#include <graphics/systems/camera.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/platform/platform.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/components/world_matrix.h>

using graphics::components::camera::ProjectionType;
using graphics::components::camera::Camera;
using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;
using graphics::systems::ecs_observers::get_app;

namespace graphics::systems::camera
{

    glm::mat4 compute_projection(const Camera& cam) 
    {
        if (cam.type == ProjectionType::Perspective) {
            return glm::perspective(
                cam.fov,
                cam.aspect,
                cam.nearPlane,
                cam.farPlane
            );
        }
        else {
            float halfH = cam.orthoHeight * 0.5f;
            float halfW = halfH * cam.aspect;

            return glm::ortho(
                -halfW, halfW,
                -halfH, halfH,
                cam.nearPlane,
                cam.farPlane
            );
        }
    }

    glm::vec3 get_forward(const entt::registry& reg, entt::entity e)
    {
        const auto& wm = reg.get<WorldMatrix>(e);
        return -glm::vec3(wm.value[2]);
    }

    glm::vec3 get_right(const entt::registry& reg, entt::entity e)
    {
        const auto& wm = reg.get<WorldMatrix>(e);
        return glm::vec3(wm.value[0]);
    }

    glm::vec3 get_up(const entt::registry& reg, entt::entity e)
    {
        const auto& wm = reg.get<WorldMatrix>(e);
        return glm::vec3(wm.value[1]);
    }

    void apply_mouse_look(Transform& t, float dx, float dy, float sensitivity) 
    {
        glm::vec3 rot = t.get_rotation();

        rot.y -= dx * sensitivity; // yaw
        rot.x += dy * sensitivity; // pitch

        float pitchLimit = glm::radians(89.0f);
        rot.x = glm::clamp(rot.x, -pitchLimit, pitchLimit);

        t.set_rotation(rot);
    }

    void move_camera_forward(Transform& t, const entt::registry& reg, entt::entity cam, float amount) 
    {
        t.set_position(t.get_position() + get_forward(reg, cam) * amount);
    }

    void move_camera_right(Transform& t, const entt::registry& reg, entt::entity cam, float amount) 
    {
        t.set_position(t.get_position() + get_right(reg, cam) * amount);
    }

    void move_camera_up(Transform& t, const entt::registry& reg, entt::entity cam, float amount) 
    {
        t.set_position(t.get_position() + get_up(reg, cam) * amount);
    }

    void update_camera_system(entt::registry& reg)
    {
        auto view = reg.view<Camera, Transform>();
        for (auto [entity, camera, transform] : view.each())
        {
            if (camera.primary)
            {
                auto& app = get_app(reg);
                if (app.input.mouse.right_is_down)
                    apply_mouse_look(transform, static_cast<float>(app.input.mouse.dx), static_cast<float>(-1.*app.input.mouse.dy), 0.002f);
                if (app.input.scroll.y != 0.0f) {
                    float zoomSpeed = 0.1f * (camera.type == ProjectionType::Orthographic ? camera.orthoHeight : camera.fov); // tune or scale based on FOV/orthoHeight
                    zoom_camera(reg, entity, app.input.scroll.y * zoomSpeed);
                    app.input.scroll.y = 0.0f; // consume
                }

                float speed = 5.0f * static_cast<float>(app.time.delta_time);

                if (app.input.keys.is_down(GLFW_KEY_W))
                    move_camera_forward(transform, reg, entity, speed);

                if (app.input.keys.is_down(GLFW_KEY_S))
                    move_camera_forward(transform, reg, entity, -speed);

                if (app.input.keys.is_down(GLFW_KEY_D))
                    move_camera_right(transform, reg, entity, speed);

                if (app.input.keys.is_down(GLFW_KEY_A))
                    move_camera_right(transform, reg, entity, -speed);

                if (app.input.keys.is_down(GLFW_KEY_E))
                    move_camera_up(transform, reg, entity, speed);

                if (app.input.keys.is_down(GLFW_KEY_Q))
                    move_camera_up(transform, reg, entity, -speed);
            }
        }
    }

    void zoom_camera(entt::registry& reg, entt::entity camEntity, float amount)
    {
        auto& cam = reg.get<Camera>(camEntity);

        if (cam.type == ProjectionType::Perspective) 
        {
            // Perspective zoom = adjust FOV
            cam.fov -= amount;
            cam.fov = glm::clamp(cam.fov, glm::radians(5.0f), glm::radians(120.0f));
        }
        else 
        {
            // Orthographic zoom = adjust orthoHeight
            cam.orthoHeight -= amount;
            cam.orthoHeight = std::max(0.01f, cam.orthoHeight);
        }
    }

}
