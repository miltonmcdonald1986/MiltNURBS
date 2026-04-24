#include <graphics/factories/mesh_factories.h>

#include <format>

#include <graphics/mesh/vertex_layout.h>
#include <graphics/mesh/mesh_factory_backend.h>

using graphics::components::mesh_gl::MeshGL;
using graphics::mesh::vertex_layout::VertexLayout;
using graphics::mesh::mesh_factory_backend::create_indexed_mesh_gl_layout;
using graphics::mesh::mesh_factory_backend::create_mesh_gl_layout;
using graphics::mesh::mesh_factory_backend::create_mesh_gl_pos_only;

namespace graphics::factories::mesh_factories
{

	std::expected<MeshGL, std::string> create_rainbow_triangle_mesh()
	{
        VertexLayout PosColorLayout{
            .stride = 6 * sizeof(float), // 3 pos + 3 color
            .attributes = {
                { 0, 3, 0 },                        // position at offset 0
                { 1, 3, 3 * sizeof(float) }         // color after 3 floats
            }
        };

        float vertices[] = {
            // pos              // color
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // red
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // green
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // blue
        };

        unsigned int indices[] = {
            0, 1, 2
        };

        auto meshResult = create_indexed_mesh_gl_layout(vertices, indices, PosColorLayout);
        if (!meshResult)
            return std::unexpected(std::format("Failed to create mesh: {}\n", meshResult.error()));

		return *meshResult;
	}

    std::expected<MeshGL, std::string> create_textured_quad_mesh()
    {
        // Interleaved vertex data: position (3 floats) + uv (2 floats)
        const float vertices[] =
        {
            //   x      y     z      u     v
            -0.5f, -0.5f, 0.f,   0.f, 0.f, // bottom-left
             0.5f, -0.5f, 0.f,   1.f, 0.f, // bottom-right
             0.5f,  0.5f, 0.f,   1.f, 1.f, // top-right
            -0.5f,  0.5f, 0.f,   0.f, 1.f  // top-left
        };

        const unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        // Vertex layout: vec3 position + vec2 uv
        VertexLayout layout;
        layout.stride = sizeof(float) * 5;

        layout.attributes.push_back({
            /* index      */ 0,
            /* components */ 3,
            /* offset     */ 0
            });

        layout.attributes.push_back({
            /* index      */ 1,
            /* components */ 2,
            /* offset     */ sizeof(float) * 3
            });

        return create_indexed_mesh_gl_layout(
            std::span<const float>(vertices, std::size(vertices)),
            std::span<const unsigned int>(indices, std::size(indices)),
            layout,
            GL_TRIANGLES
        );
    }

    std::expected<MeshGL, std::string> create_textured_triangle_mesh()
    {
        // Position + UV layout: 3 floats pos, 2 floats uv
        VertexLayout PosUvLayout{
            .stride = 5 * sizeof(float),
            .attributes = {
                { 0, 3, 0 },                        // position at offset 0
                { 1, 2, 3 * sizeof(float) }         // uv at offset 3 floats
            }
        };

        static constexpr float vertices[] = {
            // pos               // uv
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
             0.0f,  0.5f, 0.0f,   0.5f, 1.0f
        };

        return create_mesh_gl_layout(vertices, PosUvLayout, GL_TRIANGLES);
    }

    std::expected<MeshGL, std::string> create_triangle_mesh()
    {
        // 3 vertices, each with 3 floats (x, y, z)
        static constexpr float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        return create_mesh_gl_pos_only(vertices, 3, GL_TRIANGLES);
    }

} // namespace graphics::factories::mesh_factories
