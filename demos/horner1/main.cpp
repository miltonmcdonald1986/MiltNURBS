#include <graphics/camera/camera.h>
#include <graphics/camera/camera_controller.h>
#include <graphics/camera/camera_controller_state.h>
#include <graphics/camera/camera_factory.h>
#include <graphics/camera/camera_matrices.h>
#include <graphics/camera/orthographic_camera.h>
#include <graphics/camera/perspective_camera.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/parent.h>
#include <graphics/components/shader.h>
#include <graphics/components/shake.h>
#include <graphics/components/tags.h>
#include <graphics/components/texture.h>
#include <graphics/components/transform.h>
#include <graphics/engine/app_data.h>
#include <graphics/engine/engine.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/platform/glfw_callbacks.h>
#include <graphics/platform/window.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/animation.h>
#include <graphics/ui/entity_list.h>
#include <graphics/ui/inspector.h>
#include <graphics/ui/widgets.h>

using graphics::camera::Camera;
using graphics::camera::CameraController;
using graphics::camera::CameraControllerState;
using graphics::camera::CameraMatrices;
using graphics::camera::OrthographicCamera;
using graphics::camera::PerspectiveCamera;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::parent::Parent;
using graphics::components::shader::Shader;
using graphics::components::shake::Shake;
using graphics::components::shake::ShakeOnce;
using graphics::components::tags::Shakeable;
using graphics::components::texture::Texture;
using graphics::components::transform::Transform;
using graphics::engine::AppData;
using graphics::engine::run;
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
using graphics::ui::entity_list::draw_entity_list;
using graphics::ui::inspector::draw_inspector;
using graphics::ui::widgets::draw_flash_widget;
using graphics::ui::widgets::draw_shake_widget;
using graphics::ui::widgets::draw_shake_once_widget;
using graphics::ui::widgets::draw_per_entity_color_widget;
using graphics::ui::widgets::draw_render_settings_widget;

graphics::engine::Status init(AppData* p_data)
{
    if (!p_data)
        return std::unexpected(ERR("No app data found", "horner1"));

    Scene* p_scene = p_data->p_active_scene;
    if (!p_scene)
        return std::unexpected(ERR("No active scene found", "horner1"));

    Window* p_window = p_data->p_window;
    if (!p_window)
        return std::unexpected(ERR("No window found", "horner1"));

    entt::registry& reg = p_scene->reg;

    for (int i = 0; i < 5; i++) {
        entt::entity e = reg.create();
        reg.emplace<Transform>(e,
            glm::vec3(i * 2.0f, 0, -5.0f),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
        );
        reg.emplace<graphics::components::Color>(e, graphics::components::Color{});
        reg.emplace<graphics::components::Flash>(e, graphics::components::Flash{});
        reg.emplace<MeshGL>(e, *create_textured_cube_mesh());
        reg.emplace<Shader>(e, *create_textured_color_mvp_shader());
        if (auto tex_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
            reg.emplace<Texture>(e, *tex_result);
        else
            return std::unexpected(ERR("Failed to create texture.", "horner1"));
    }

    entt::entity camera = reg.create();
    graphics::camera::CameraConfig camera_config{};
    graphics::camera::add_camera(reg, camera, camera_config);
    
    return {};
}

graphics::engine::Status update(AppData* p_data)
{
    if (!p_data)
        return std::unexpected(ERR("No app data found", "horner1"));

    // Optional UI
    draw_entity_list(p_data);
    draw_inspector(p_data);

    return {};
}

int main(void)
{
    auto result = run(init, update);
    return result.has_value() ? 0 : 1;
}
