#ifndef GRAPHICS_MESH_MESH_FACTORY_BACKEND_H
#define GRAPHICS_MESH_MESH_FACTORY_BACKEND_H

#include <expected>
#include <string>
#include <span>

#include <graphics/components/mesh_gl.h>
#include <graphics/mesh/vertex_layout.h>

namespace graphics::mesh::mesh_factory_backend
{
	
	std::expected<graphics::components::mesh_gl::MeshGL, std::string> create_indexed_mesh_gl_layout(std::span<const float> vertices, std::span<const unsigned int> indices, const graphics::mesh::vertex_layout::VertexLayout& layout, GLenum primitive = GL_TRIANGLES);
    std::expected<graphics::components::mesh_gl::MeshGL, std::string> create_indexed_mesh_gl_pos_only(std::span<const float> vertices, std::span<const unsigned int> indices, GLint componentsPerVertex, GLenum primitive);
	std::expected<graphics::components::mesh_gl::MeshGL, std::string> create_mesh_gl_layout(std::span<const float> vertices, const graphics::mesh::vertex_layout::VertexLayout& layout, GLenum primitive = GL_TRIANGLES);
	std::expected<graphics::components::mesh_gl::MeshGL, std::string> create_mesh_gl_pos_only(std::span<const float> vertices, GLint componentsPerVertex = 3, GLenum primitive = GL_TRIANGLES);

} // namespace graphics::mesh::mesh_factory_backend

#endif // GRAPHICS_MESH_MESH_FACTORY_BACKEND_H
