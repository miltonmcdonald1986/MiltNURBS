#ifndef GRAPHICS_SYSTEMS_ECS_OBSERVERS_H
#define GRAPHICS_SYSTEMS_ECS_OBSERVERS_H

#include <entt/entt.hpp>

// Forward declarations
namespace graphics::engine { struct AppData; }

namespace graphics::systems
{

	engine::AppData* get_app(const entt::registry& p_reg);
	void register_transform_observers(engine::AppData* p_data);

}

#endif // GRAPHICS_SYSTEMS_ECS_OBSERVERS_H
