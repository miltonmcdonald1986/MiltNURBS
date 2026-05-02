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

    void update_input_system(entt::registry& reg);

}

#endif // GRAPHICS_INPUT_INPUT_SYSTEMS_H
