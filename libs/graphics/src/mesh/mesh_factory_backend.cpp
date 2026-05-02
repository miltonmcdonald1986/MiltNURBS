#include <graphics/mesh/mesh_factory_backend.h>

inline const std::string NAMESPACE = "graphics::mesh";

namespace graphics::mesh
{

    engine::Result<components::MeshGL> create_indexed_mesh_gl_layout(std::span<const float> vertices, std::span<const unsigned int> indices, const VertexLayout& layout, GLenum primitive)
    {
        components::MeshGL mesh{};
        mesh.primitive = primitive;
        mesh.indexCount = static_cast<GLsizei>(indices.size());

        // Validate vertex count
        if ((vertices.size() * sizeof(float)) % layout.stride != 0)
            return std::unexpected(ERR("Vertex data does not align with layout stride"));

        // --- VAO ---
        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        // --- VBO ---
        glGenBuffers(1, &mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

        // --- EBO ---
        glGenBuffers(1, &mesh.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW);

        // --- Attributes ---
        for (const auto& attr : layout.attributes)
        {
            glVertexAttribPointer(
                attr.index,
                attr.components,
                GL_FLOAT,
                GL_FALSE,
                static_cast<GLsizei>(layout.stride),
                reinterpret_cast<const void*>(attr.offset)
            );
            glEnableVertexAttribArray(attr.index);
        }

        glBindVertexArray(0);

        if (glGetError() != GL_NO_ERROR)
            return std::unexpected(ERR("Failed to create MeshGL with custom layout"));

        return mesh;
    }

    engine::Result<components::MeshGL> create_indexed_mesh_gl_pos_only(std::span<const float> vertices, std::span<const unsigned int> indices, GLint componentsPerVertex, GLenum primitive)
    {
        VertexLayout layout{};
        layout.stride = componentsPerVertex * sizeof(float);

        layout.attributes.push_back({
            .index = 0,
            .components = componentsPerVertex,
            .offset = 0
            });

        return create_indexed_mesh_gl_layout(vertices, indices, layout, primitive);
    }

    engine::Result<components::MeshGL> create_mesh_gl_layout(std::span<const float> vertices, const VertexLayout& layout, GLenum primitive)
    {
        components::MeshGL mesh{};
        mesh.primitive = primitive;

        // Compute vertex count
        if (layout.stride == 0)
            return std::unexpected(ERR("VertexLayout.stride must not be zero"));

        if ((vertices.size() * sizeof(float)) % layout.stride != 0)
            return std::unexpected(ERR("Vertex data size does not match vertex stride"));

        mesh.vertexCount =
            static_cast<GLsizei>((vertices.size() * sizeof(float)) / layout.stride);

        // --- VAO ---
        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        // --- VBO ---
        glGenBuffers(1, &mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

        // --- Vertex Attributes ---
        for (const auto& attr : layout.attributes)
        {
            glEnableVertexAttribArray(attr.index);
            glVertexAttribPointer(
                attr.index,
                attr.components,                 // vec2, vec3, vec4
                GL_FLOAT,
                GL_FALSE,
                static_cast<GLsizei>(layout.stride),
                reinterpret_cast<const void*>(attr.offset)
            );
        }

        glBindVertexArray(0);

        if (glGetError() != GL_NO_ERROR)
            return std::unexpected(ERR("Failed to create MeshGL"));

        return mesh;
    }

    engine::Result<components::MeshGL> create_mesh_gl_pos_only(std::span<const float> vertices, GLint componentsPerVertex, GLenum primitive)
    {
        VertexLayout layout{};
        layout.stride = componentsPerVertex * sizeof(float);
        layout.attributes.push_back({
            .index = 0,
            .components = componentsPerVertex,
            .offset = 0
            });

        return create_mesh_gl_layout(vertices, layout, primitive);
    }

}
