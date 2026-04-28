#ifndef GRAPHICS_COMPONENTS_WORLD_MATRIX_H
#define GRAPHICS_COMPONENTS_WORLD_MATRIX_H

#include <glm/glm.hpp>

namespace graphics::components::world_matrix
{

	struct WorldMatrix
	{
		glm::mat4 value{ 1.0f };
	};

}

#endif // GRAPHICS_COMPONENTS_WORLD_MATRIX_H
