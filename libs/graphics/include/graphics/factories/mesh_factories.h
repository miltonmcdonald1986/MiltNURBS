#ifndef GRAPHICS_FACTORIES_MESH_FACTORIES_H
#define GRAPHICS_FACTORIES_MESH_FACTORIES_H

#include <expected>
#include <string>

#include <graphics/components/mesh_gl.h>
#include <graphics/engine/result.h>

namespace graphics::factories
{

	engine::Result<components::MeshGL> create_rainbow_triangle_mesh();
	engine::Result<components::MeshGL> create_textured_cube_mesh();
	engine::Result<components::MeshGL> create_textured_quad_mesh();
	engine::Result<components::MeshGL> create_textured_triangle_mesh();
	engine::Result<components::MeshGL> create_triangle_mesh();

}

#endif // GRAPHICS_FACTORIES_MESH_FACTORIES_H
