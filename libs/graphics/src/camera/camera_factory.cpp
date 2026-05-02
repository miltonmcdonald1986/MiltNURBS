#include <graphics/camera/camera_factory.h>

#include <graphics/camera/camera_matrices.h>
#include <graphics/camera/camera_controller_state.h>
#include <graphics/components/transform.h>

namespace graphics::camera
{

	void add_camera(entt::registry& reg, entt::entity ent, const CameraConfig& config)
	{
		reg.emplace<Camera>(ent, config.camera);
		reg.emplace<CameraController>(ent, config.controller);
		reg.emplace<components::Transform>(ent, config.transform);
		reg.emplace<CameraControllerState>(ent, CameraControllerState{});
		reg.emplace<CameraMatrices>(ent, CameraMatrices{});
		switch (config.camera.type)
		{
		case ProjectionType::Orthographic:
			reg.emplace<OrthographicCamera>(ent, config.orthographic);
			break;
		case ProjectionType::Perspective:
			reg.emplace<PerspectiveCamera>(ent, config.perspective);
			break;
		}
	}

}
