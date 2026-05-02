#ifndef GRAPHICS_FACTORIES_PREFAB_FACTORIES_H
#define GRAPHICS_FACTORIES_PREFAB_FACTORIES_H

#include <expected>

#include <entt/entt.hpp>

#include <graphics/components/color.h>
#include <graphics/engine/result.h>

namespace graphics::factories
{

	engine::Result<entt::entity> create_solid_color_triangle_ent(entt::registry& reg, const components::Color& color);
	engine::Result<entt::entity> create_rainbow_triangle_ent(entt::registry& reg);

}

#endif // GRAPHICS_FACTORIES_PREFAB_FACTORIES_H
