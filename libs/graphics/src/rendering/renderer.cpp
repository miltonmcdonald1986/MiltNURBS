#include <graphics/rendering/renderer.h>

#include <glm/gtc/type_ptr.hpp>

#include <graphics/components/camera.h>
#include <graphics/components/color.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/texture.h>
#include <graphics/components/world_matrix.h>
#include <graphics/platform/gl_includes.h>
#include <graphics/systems/camera.h>

using graphics::components::camera::Camera;
using graphics::components::color::Color;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::texture::Texture;
using graphics::components::world_matrix::WorldMatrix;
using graphics::scene::Scene;
using graphics::systems::camera::compute_projection;

namespace graphics::rendering::renderer
{

    std::expected<void, std::string> Renderer::init(int framebuffer_width, int framebuffer_height)
    {
        glViewport(0, 0, framebuffer_width, framebuffer_height);

        if (gl_state.enable_depth_test)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(gl_state.depth_func);
        }

        glPolygonMode(GL_FRONT_AND_BACK, gl_state.polygon_mode);

        auto& clearColor = gl_state.clear_color;
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

        // Check for any OpenGL errors that may have occurred during initialization.
        if (glGetError() != GL_NO_ERROR)
            return std::unexpected("Failed to init GL state");

        return {};
    }

    std::expected<void, std::string> Renderer::update(Scene* p_scene)
    {
        if (!p_scene)
            return std::unexpected("No active scene found");

        entt::registry& reg = p_scene->reg;

        glClearColor(
            gl_state.clear_color[0],
            gl_state.clear_color[1],
            gl_state.clear_color[2],
            gl_state.clear_color[3]
        );

        glClear(gl_state.clear_buffer_mask);

        if (gl_state.display_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // ---------------------------------------------------------
        // 1. Find the active camera
        // ---------------------------------------------------------
        entt::entity cam_ent = entt::null;
        auto cam_view = reg.view<Camera, WorldMatrix>();
        for (auto [ent, camera, world_matrix] : cam_view.each())
        {
            if (camera.primary) 
            {
                cam_ent = ent;
                break;
            }
        }

        if (cam_ent == entt::null) 
        {
            return std::unexpected("No active camera found");
        }

        auto& cam = reg.get<Camera>(cam_ent);
        auto& camWM = reg.get<WorldMatrix>(cam_ent);

        // ---------------------------------------------------------
        // 2. Compute view + projection
        // ---------------------------------------------------------
        glm::mat4 view = glm::inverse(camWM.value);
        glm::mat4 proj = compute_projection(cam);

        // ---------------------------------------------------------
        // 3. Render all meshes
        // ---------------------------------------------------------
        auto ents = reg.view<Shader, MeshGL>();
        for (auto [entity, shader, mesh] : ents.each())
        {
            glUseProgram(shader.id);

            // --- Optional Color uniform ---
            if (auto color = reg.try_get<Color>(entity))
            {
                GLint loc = glGetUniformLocation(shader.id, "u_color");
                if (loc >= 0)
                    glUniform4fv(loc, 1, color->rgba);
            }

            // --- Optional Texture binding ---
            if (auto tex = reg.try_get<Texture>(entity))
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex->id);

                GLint loc = glGetUniformLocation(shader.id, "uTexture");
                if (loc >= 0)
                    glUniform1i(loc, 0);
            }

            // --- Upload uModel ---
            if (GLint loc = glGetUniformLocation(shader.id, "uModel"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(reg.get<WorldMatrix>(entity).value));

            // --- Upload uView ---
            if (GLint loc = glGetUniformLocation(shader.id, "uView"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

            // --- Upload uProjection ---
            if (GLint loc = glGetUniformLocation(shader.id, "uProjection"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj));

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
