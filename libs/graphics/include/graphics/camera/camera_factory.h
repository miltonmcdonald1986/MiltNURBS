#ifndef GRAPHICS_CAMERA_CAMERA_FACTORY_H
#define GRAPHICS_CAMERA_CAMERA_FACTORY_H

#include <entt/entt.hpp>

#include <glm/glm.hpp>

#include <graphics/camera/camera.h>
#include <graphics/camera/camera_controller.h>
#include <graphics/camera/orthographic_camera.h>
#include <graphics/camera/perspective_camera.h>
#include <graphics/camera/projection_type.h>
#include <graphics/components/transform.h>

namespace graphics::camera
{

	struct CameraConfig
	{
		Camera camera{};
		CameraController controller{};
		components::Transform transform{ components::Transform(glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f), glm::vec3(1.f)) };
		OrthographicCamera orthographic{};
		PerspectiveCamera perspective{};
	};

	void add_camera(entt::registry& reg, entt::entity ent, const CameraConfig& config);

}

#endif // GRAPHICS_CAMERA_CAMERA_FACTORY_H
