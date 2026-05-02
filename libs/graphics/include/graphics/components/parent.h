#ifndef GRAPHICS_COMPONENTS_PARENT_H
#define GRAPHICS_COMPONENTS_PARENT_H

#include <entt/entt.hpp>

namespace graphics::components
{

	struct Parent
	{
		entt::entity parent = entt::null;
	};

}

#endif // GRAPHICS_COMPONENTS_PARENT_H
