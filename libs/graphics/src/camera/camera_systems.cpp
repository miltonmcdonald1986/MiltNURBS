#include <graphics/camera/camera_systems.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/camera/camera.h>
#include <graphics/camera/camera_controller.h>
#include <graphics/camera/camera_controller_state.h>
#include <graphics/camera/orthographic_camera.h>
#include <graphics/camera/perspective_camera.h>
#include <graphics/engine/app_data.h>
#include <graphics/input/key.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/components/transform.h>
#include <graphics/components/world_matrix.h>

using graphics::camera::Camera;
using graphics::camera::CameraController;
using graphics::camera::CameraControllerState;
using graphics::camera::OrthographicCamera;
using graphics::camera::PerspectiveCamera;
using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;
using graphics::engine::AppData;
using graphics::input::Key;
using graphics::systems::ecs_observers::get_app;

namespace
{

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

    void look(Transform& t, float dx, float dy, float sensitivity, float pitch_limit)
    {
        glm::vec3 rot = t.get_rotation();

        rot.y -= dx * sensitivity; // yaw
        rot.x += dy * sensitivity; // pitch

        const float pitch_limit_rads = glm::radians(pitch_limit);
        rot.x = glm::clamp(rot.x, -pitch_limit_rads, pitch_limit_rads);

        t.set_rotation(rot);
    }

    void move(Transform& t, const entt::registry& reg, entt::entity cam, const glm::vec3& amount)
    {
        t.set_position(t.get_position() + get_right(reg, cam) * amount.x);
        t.set_position(t.get_position() + get_up(reg, cam) * amount.y);
        t.set_position(t.get_position() + get_forward(reg, cam) * amount.z);
    }

    void zoom_scroll(entt::registry& reg, entt::entity cam_ent, float scroll, float speed)
    {
        if (Camera* p_camera = reg.try_get<Camera>(cam_ent))
        {
            switch (p_camera->type)
            {
            case Camera::ProjectionType::Orthographic:
            {
                if (OrthographicCamera* p_ortho = reg.try_get<OrthographicCamera>(cam_ent))
                {
                    float scaled_speed = speed * p_ortho->height;
                    p_ortho->height -= (scroll * scaled_speed);
                    p_ortho->height = std::max(0.01f, p_ortho->height);
                }
                break;
            }
            case Camera::ProjectionType::Perspective:
            {
                if (PerspectiveCamera* p_persp = reg.try_get<PerspectiveCamera>(cam_ent))
                {
                    float scaled_speed = speed * p_persp->fov;
                    p_persp->fov -= (scroll * scaled_speed);
                    p_persp->fov = glm::clamp(p_persp->fov, 5.0f, 120.0f);
                }
                break;
            }
            }
        }
    }

}

namespace graphics::camera
{

    glm::mat4 compute_projection(const entt::registry& reg, entt::entity cam_ent, float aspect)
    {
        if (const Camera* p_camera = reg.try_get<Camera>(cam_ent))
        {
            switch (p_camera->type)
            {
            case Camera::ProjectionType::Orthographic:
            {
                if (const OrthographicCamera* p_ortho = reg.try_get<OrthographicCamera>(cam_ent))
                {
                    float halfH = p_ortho->height * 0.5f;
                    float halfW = halfH * aspect;

                    return glm::ortho(-halfW, halfW, -halfH, halfH, p_ortho->clip_planes.near, p_ortho->clip_planes.far);
                }

                break;
            }
            case Camera::ProjectionType::Perspective:
            {
                if (const PerspectiveCamera* p_persp = reg.try_get<PerspectiveCamera>(cam_ent))
                    return glm::perspective(glm::radians(p_persp->fov), aspect, p_persp->clip_planes.near, p_persp->clip_planes.far);
         
                break;
            }
            }
        }

        return glm::mat4(1.f);
    }

    void update_camera_system(entt::registry& reg, float dt)
    {
        auto view = reg.view<Camera, CameraController, CameraControllerState, Transform>();
        for (auto [cam_ent, camera, camera_controller, camera_controller_state, transform] : view.each())
        {
            if (camera_controller.enable_look && camera_controller_state.look_active)
                look(transform, camera_controller_state.look_delta.x, -1.f * camera_controller_state.look_delta.y, camera_controller.look_sensitivity, camera_controller.pitch_limit);

            if (camera_controller.enable_zoom && camera_controller_state.zoom_delta != 0.f)
                zoom_scroll(reg, cam_ent, camera_controller_state.zoom_delta, camera_controller.zoom_speed);

            if (camera_controller.enable_movement)
            {
                glm::vec3 total_movement = camera_controller_state.move_delta * camera_controller.move_speed * dt;
                move(transform, reg, cam_ent, total_movement);
            }
        }
    }

}
