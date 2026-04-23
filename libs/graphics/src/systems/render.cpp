#include <graphics/systems/render.h>

#include <graphics/components/color.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/texture.h>

using graphics::app::app::App;
using graphics::components::color::Color;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::texture::Texture;

namespace graphics::systems::render
{

    std::expected<void, std::string> render_system_update(App& app)
    {
        glClearColor(
            app.glState.clear_color[0],
            app.glState.clear_color[1],
            app.glState.clear_color[2],
            app.glState.clear_color[3]
        );

        glClear(app.glState.clear_buffer_mask);

        if (app.glState.display_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        auto ents = app.reg.view<Shader, MeshGL>();
        for (auto [entity, shader, mesh] : ents.each())
        {
            glUseProgram(shader.id);

            // --- Optional Color uniform ---
            if (auto color = app.reg.try_get<Color>(entity))
            {
                GLint loc = glGetUniformLocation(shader.id, "u_color");
                if (loc >= 0)
                    glUniform4fv(loc, 1, color->rgba);
            }

            // --- Optional Texture binding ---
            if (auto tex = app.reg.try_get<Texture>(entity))
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex->id);

                GLint loc = glGetUniformLocation(shader.id, "uTexture");
                if (loc >= 0)
                    glUniform1i(loc, 0); // texture unit 0
            }

            // --- Draw ---
            glBindVertexArray(mesh.vao);

            if (mesh.indexCount > 0)
                glDrawElements(mesh.primitive, mesh.indexCount, GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(mesh.primitive, 0, mesh.vertexCount);
        }

        return {};
    }

}
