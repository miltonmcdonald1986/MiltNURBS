#ifndef GRAPHICS_INPUT_INPUT_SYSTEMS_H
#define GRAPHICS_INPUT_INPUT_SYSTEMS_H

#include <entt/entt.hpp>

#include <glm/glm.hpp>

#include <graphics/camera/camera_controller.h>
#include <graphics/camera/camera_controller_state.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/engine/app_data.h>

namespace graphics::input
{

    void update_input_system(entt::registry& reg)
    {
        engine::AppData* p_data = systems::ecs_observers::get_app(reg);
        if (!p_data)
            return;

        auto& input = p_data->input;

        auto view = reg.view<camera::CameraController, camera::CameraControllerState>();
        for (auto [ent, controller, state] : view.each())
        {
            // Reset state
            state.move_delta = glm::vec3(0.f);
            state.look_delta = glm::vec2(0.f);
            state.zoom_delta = 0.f;

            // LOOK
            state.look_active = input.keys[controller.look_activation];
            if (controller.enable_look)
                state.look_delta = { (float)input.mouse.dx, (float)input.mouse.dy };

            // MOVE
            if (controller.enable_movement)
            {
                if (input.keys[controller.forward])  state.move_delta.z += 1.f;
                if (input.keys[controller.backward]) state.move_delta.z -= 1.f;
                if (input.keys[controller.right])    state.move_delta.x += 1.f;
                if (input.keys[controller.left])     state.move_delta.x -= 1.f;
                if (input.keys[controller.up])       state.move_delta.y += 1.f;
                if (input.keys[controller.down])     state.move_delta.y -= 1.f;
            }

            // ZOOM
            if (controller.enable_zoom)
                state.zoom_delta = (float)input.scroll.y;
        }

        // ⭐ Reset scroll AFTER all controllers have consumed it
        input.scroll.y = 0.0;
    }

}

#endif // GRAPHICS_INPUT_INPUT_SYSTEMS_H
