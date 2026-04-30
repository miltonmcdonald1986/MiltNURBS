#ifndef GRAPHICS_SYSTEMS_ECS_OBSERVERS_H
#define GRAPHICS_SYSTEMS_ECS_OBSERVERS_H

#include <entt/entt.hpp>

#include <graphics/app/app.h>

namespace graphics::systems::ecs_observers
{

	app::app::App& get_app(entt::registry& reg);
	void register_transform_observers(app::app::App& app);

}

#endif // GRAPHICS_SYSTEMS_ECS_OBSERVERS_H
