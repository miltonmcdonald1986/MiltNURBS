#include <graphics/camera/camera_systems.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/camera/camera.h>
#include <graphics/camera/camera_controller.h>
#include <graphics/camera/camera_controller_state.h>
#include <graphics/camera/camera_matrices.h>
#include <graphics/camera/orthographic_camera.h>
#include <graphics/camera/perspective_camera.h>
#include <graphics/engine/app_data.h>
#include <graphics/input/key.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/components/transform.h>
#include <graphics/components/world_matrix.h>
#include <graphics/platform/window.h>
#include <graphics/utils/math_utils.h>

namespace graphics::camera
{

	glm::vec3 get_forward(const entt::registry& reg, entt::entity e)
	{
		const auto& wm = reg.get<components::WorldMatrix>(e);
		return -glm::vec3(wm.value[2]);
	}

	glm::vec3 get_right(const entt::registry& reg, entt::entity e)
	{
		const auto& wm = reg.get<components::WorldMatrix>(e);
		return glm::vec3(wm.value[0]);
	}

	glm::vec3 get_up(const entt::registry& reg, entt::entity e)
	{
		const auto& wm = reg.get<components::WorldMatrix>(e);
		return glm::vec3(wm.value[1]);
	}

	void look(components::Transform& t, float dx, float dy, float sensitivity, float pitch_limit)
	{
		glm::vec3 rot = t.get_rotation();

		rot.y -= dx * sensitivity; // yaw
		rot.x += dy * sensitivity; // pitch

		const float pitch_limit_rads = glm::radians(pitch_limit);
		rot.x = glm::clamp(rot.x, -pitch_limit_rads, pitch_limit_rads);

		t.set_rotation(rot);
	}

	void move(components::Transform& t, const entt::registry& reg, entt::entity cam, const glm::vec3& amount)
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
			case ProjectionType::Orthographic:
			{
				if (OrthographicCamera* p_ortho = reg.try_get<OrthographicCamera>(cam_ent))
				{
					float scaled_speed = speed * p_ortho->height;
					p_ortho->height -= (scroll * scaled_speed);
					p_ortho->height = std::max(0.01f, p_ortho->height);
				}
				break;
			}
			case ProjectionType::Perspective:
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

	glm::mat4 compute_projection(const entt::registry& reg, entt::entity cam_ent, float aspect)
	{
		if (const Camera* p_camera = reg.try_get<Camera>(cam_ent))
		{
			switch (p_camera->type)
			{
			case ProjectionType::Orthographic:
			{
				if (const OrthographicCamera* p_ortho = reg.try_get<OrthographicCamera>(cam_ent))
				{
					float halfH = p_ortho->height * 0.5f;
					float halfW = halfH * aspect;

					return glm::ortho(-halfW, halfW, -halfH, halfH, p_ortho->clip_planes.z_near, p_ortho->clip_planes.z_far);
				}

				break;
			}
			case ProjectionType::Perspective:
			{
				if (const PerspectiveCamera* p_persp = reg.try_get<PerspectiveCamera>(cam_ent))
					return glm::perspective(glm::radians(p_persp->fov), aspect, p_persp->clip_planes.z_near, p_persp->clip_planes.z_far);

				break;
			}
			}
		}

		return glm::mat4(1.f);
	}

	void update_camera_system(entt::registry& reg, float dt)
	{
		engine::AppData* p_data = systems::get_app(reg);
		if (!p_data)
			return;

		auto view = reg.view<Camera, CameraController, CameraControllerState, CameraMatrices, components::Transform>();
		for (auto [cam_ent, camera, camera_controller, camera_controller_state, camera_matrices, transform] : view.each())
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

			auto& camWM = reg.get<components::WorldMatrix>(cam_ent);
			auto worldNoScale = utils::remove_scale_from_matrix(camWM.value);

			camera_matrices.view = glm::inverse(worldNoScale);
			camera_matrices.projection = compute_projection(reg, cam_ent, p_data->p_window->window_state.aspect());
		}
	}

}
