#ifndef GRAPHICS_MESH_MESH_FACTORY_BACKEND_H
#define GRAPHICS_MESH_MESH_FACTORY_BACKEND_H

#include <expected>
#include <string>
#include <span>

#include <graphics/components/mesh_gl.h>
#include <graphics/mesh/vertex_layout.h>

namespace graphics::mesh
{
	
	std::expected<components::MeshGL, std::string> create_indexed_mesh_gl_layout(std::span<const float> vertices, std::span<const unsigned int> indices, const mesh::VertexLayout& layout, GLenum primitive = GL_TRIANGLES);
    std::expected<components::MeshGL, std::string> create_indexed_mesh_gl_pos_only(std::span<const float> vertices, std::span<const unsigned int> indices, GLint componentsPerVertex, GLenum primitive);
	std::expected<components::MeshGL, std::string> create_mesh_gl_layout(std::span<const float> vertices, const mesh::VertexLayout& layout, GLenum primitive = GL_TRIANGLES);
	std::expected<components::MeshGL, std::string> create_mesh_gl_pos_only(std::span<const float> vertices, GLint componentsPerVertex = 3, GLenum primitive = GL_TRIANGLES);

} // namespace graphics::mesh

#endif // GRAPHICS_MESH_MESH_FACTORY_BACKEND_H
