#ifndef GRAPHICS_FACTORIES_MESH_FACTORIES_H
#define GRAPHICS_FACTORIES_MESH_FACTORIES_H

#include <expected>
#include <string>

#include <graphics/components/mesh_gl.h>

namespace graphics::factories
{

	/// <summary>
	/// Creates a MeshGL representing a single triangle with interleaved
	/// position and color attributes.
	///
	/// The generated mesh contains:
	///   • 3 vertices
	///   • position (vec3) at attribute location 0
	///   • color    (vec3) at attribute location 1
	///
	/// Vertex data is tightly packed (stride = 6 floats), and the mesh
	/// is uploaded using create_indexed_mesh_gl_layout(), which allocates
	/// a VAO, VBO, and EBO and configures the provided VertexLayout.
	///
	/// On success, returns a fully initialized MeshGL ready for rendering.
	/// On failure, returns an error message describing the OpenGL upload issue.
	/// </summary>
	/// <returns>
	/// std::expected containing:
	///   • MeshGL on success
	///   • std::string error message on failure
	/// </returns>
	std::expected<components::MeshGL, std::string> create_rainbow_triangle_mesh();

	std::expected<components::MeshGL, std::string> create_textured_cube_mesh();

	/// <summary>
	/// Creates a textured quad mesh using position + UV vertex data.
	///
	/// The quad is centered at the origin in the XY plane, with a size of 1x1.
	/// Vertex layout:
	///   layout(location = 0): vec3 position
	///   layout(location = 1): vec2 uv
	///
	/// The mesh uses indexed geometry (6 indices forming two triangles):
	///   (0,1,2) and (2,3,0)
	///
	/// Suitable for sprites, UI panels, billboards, or any textured geometry
	/// requiring position + UV attributes.
	/// </summary>
	/// <returns>
	/// On success, contains a MeshGL with VAO, VBO, EBO, and index count.
	/// On failure, contains a descriptive error message.
	/// </returns>
	std::expected<components::MeshGL, std::string> create_textured_quad_mesh();

	/// <summary>
	/// Creates a MeshGL representing a triangle with position and UV attributes,
	/// suitable for rendering with a texture-sampling shader.
	///
	/// This mesh:
	///   • contains 3 vertices
	///   • each vertex stores:
	///       - position (x, y, z) at location 0
	///       - UV coordinates (u, v) at location 1
	///   • uses an interleaved vertex layout (3 floats position, 2 floats UV)
	///   • does not use an index buffer (drawn with glDrawArrays)
	///
	/// The triangle is defined in normalized device coordinates:
	///   (-0.5, -0.5, 0.0)  → UV (0.0, 0.0)
	///   ( 0.5, -0.5, 0.0)  → UV (1.0, 0.0)
	///   ( 0.0,  0.5, 0.0)  → UV (0.5, 1.0)
	///
	/// On success, returns a MeshGL containing a VAO, VBO, and vertex count.
	/// On failure, returns an error message describing the underlying issue.
	/// </summary>
	/// <returns>
	/// std::expected containing:
	///   • MeshGL on success
	///   • std::string error message on failure
	/// </returns>
	std::expected<components::MeshGL, std::string> create_textured_triangle_mesh();

	/// <summary>
	/// Creates a MeshGL representing a basic triangle with position‑only vertices.
	///
	/// This mesh:
	///   • contains 3 vertices, each with a 3‑float position (x, y, z)
	///   • uses a single vertex attribute at location 0 (position)
	///   • does not include per‑vertex color, normals, or UVs
	///   • is intended for use with shaders that supply color via a uniform
	///
	/// The triangle is defined in normalized device coordinates:
	///   (-0.5, -0.5, 0.0)
	///   ( 0.5, -0.5, 0.0)
	///   ( 0.0,  0.5, 0.0)
	///
	/// On success, returns a MeshGL containing a VAO, VBO, and vertex count.
	/// On failure, returns an error message describing the underlying issue.
	/// </summary>
	/// <returns>
	/// std::expected containing:
	///   • MeshGL on success
	///   • std::string error message on failure
	/// </returns>
	std::expected<components::MeshGL, std::string> create_triangle_mesh();

}

#endif // GRAPHICS_FACTORIES_MESH_FACTORIES_H
