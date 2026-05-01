#ifndef GRAPHICS_CAMERA_CAMERA_CONTROLLER_H
#define GRAPHICS_CAMERA_CAMERA_CONTROLLER_H

#include <graphics/input/key.h>

namespace graphics::camera
{

    struct CameraController 
    {
        // Move parameters
        bool enable_movement = true;
        float move_speed = 5.f;
        input::Key enable_movement_toggle = input::Key::Unknown;
        input::Key forward = input::Key::W;
        input::Key backward = input::Key::S;
        input::Key right = input::Key::D;
        input::Key left = input::Key::A;
        input::Key up = input::Key::E;
        input::Key down = input::Key::Q;

        // Look parameters
        bool enable_look = true;
        input::Key look_activation = input::Key::MouseRight;
        float look_sensitivity = 0.002f;
        float pitch_limit = 89.f;

        // Zoom parameters
        bool enable_zoom = true;
        float zoom_speed = 0.1f;
        input::Key enable_zoom_toggle = input::Key::Unknown;
    };

}

#endif // GRAPHICS_CAMERA_CAMERA_CONTROLLER_H
