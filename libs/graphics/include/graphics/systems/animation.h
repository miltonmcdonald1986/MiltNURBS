#ifndef GRAPHICS_SYSTEMS_ANIMATION_H
#define GRAPHICS_SYSTEMS_ANIMATION_H

#include <entt/entt.hpp>

#include <glm/glm.hpp>

namespace graphics::systems::animation
{

	void update_flash(entt::registry& reg, double dt);
	void update_shake(entt::registry& reg, double dt);
	void update_shake_once(entt::registry& reg, double dt);
	void update_shake_base_position(entt::registry& reg, entt::entity e, const glm::vec3& position);
	
}

#endif // GRAPHICS_SYSTEMS_ANIMATION_H
