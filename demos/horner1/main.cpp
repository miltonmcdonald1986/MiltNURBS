#include <graphics/app/app.h>
#include <graphics/app/app_loop.h>
#include <graphics/components/camera.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/parent.h>
#include <graphics/components/shader.h>
#include <graphics/components/shake.h>
#include <graphics/components/tags.h>
#include <graphics/components/texture.h>
#include <graphics/components/transform.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/platform/glfw_callbacks.h>
#include <graphics/platform/window.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/animation.h>
#include <graphics/systems/camera.h>
#include <graphics/ui/entity_list.h>
#include <graphics/ui/inspector.h>
#include <graphics/ui/widgets.h>

using graphics::app::app::App;
using graphics::app::app_loop::run_app;
using graphics::components::camera::Camera;
using graphics::components::camera::ProjectionType;
using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::parent::Parent;
using graphics::components::shader::Shader;
using graphics::components::shake::Shake;
using graphics::components::shake::ShakeOnce;
using graphics::components::tags::Shakeable;
using graphics::components::texture::Texture;
using graphics::components::transform::Transform;
using graphics::factories::mesh_factories::create_textured_cube_mesh;
using graphics::factories::mesh_factories::create_textured_quad_mesh;
using graphics::factories::mesh_factories::create_textured_triangle_mesh;
using graphics::factories::texture_factories::create_texture_from_file;
using graphics::factories::shader_factories::create_textured_color_mvp_shader;
using graphics::factories::shader_factories::create_textured_mvp_shader;
using graphics::platform::window::Window;
using graphics::scene::Scene;
using graphics::systems::animation::update_flash;
using graphics::systems::animation::update_shake_once;
using graphics::systems::camera::get_forward;
using graphics::systems::camera::get_right;
using graphics::systems::camera::get_up;
using graphics::systems::camera::apply_mouse_look;
using graphics::systems::camera::move_camera_forward;
using graphics::systems::camera::move_camera_right;
using graphics::systems::camera::move_camera_up;
using graphics::ui::entity_list::draw_entity_list;
using graphics::ui::inspector::draw_inspector;
using graphics::ui::widgets::draw_flash_widget;
using graphics::ui::widgets::draw_shake_widget;
using graphics::ui::widgets::draw_shake_once_widget;
using graphics::ui::widgets::draw_per_entity_color_widget;
using graphics::ui::widgets::draw_render_settings_widget;

entt::entity camera;

std::expected<void, std::string> init(App& app)
{
    Scene* p_scene = app.p_active_scene;
    if (!p_scene)
        return std::unexpected("No active scene found");

    Window* p_window = app.p_window;
    if (!p_window)
        return std::unexpected("No window found");

    entt::registry& reg = p_scene->reg;

    for (int i = 0; i < 5; i++) {
        entt::entity e = reg.create();
        reg.emplace<Transform>(e,
            glm::vec3(i * 2.0f, 0, -5.0f),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
        );
        reg.emplace<MeshGL>(e, *create_textured_cube_mesh());
        reg.emplace<Shader>(e, *create_textured_mvp_shader());
        if (auto tex_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
            reg.emplace<Texture>(e, *tex_result);
        else
            return std::unexpected("Failed to load texture");
    }

    camera = reg.create();

    reg.emplace<Transform>(camera, Transform(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

    reg.emplace<Camera>(camera, Camera{
        .type = ProjectionType::Perspective,
        .fov = glm::radians(45.f),
        .nearPlane = 0.1f,
        .farPlane = 1000.0f,
        .aspect = p_window->window_state.width / float(app.p_window->window_state.height),
        .primary = true
        });

    return {};
}

std::expected<void, std::string> update(App& app)
{
    // Optional UI
    draw_entity_list(app);
    draw_inspector(app);

    return {};
}

int main(void)
{
    return run_app(init, update);
}
