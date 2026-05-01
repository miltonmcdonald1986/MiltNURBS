#ifndef GRAPHICS_CAMERA_ORTHOGRAPHIC_CAMERA_H
#define GRAPHICS_CAMERA_ORTHOGRAPHIC_CAMERA_H

#include <graphics/camera/clip_planes.h>

namespace graphics::camera
{

    struct OrthographicCamera
    {
        float height{ 10.f };   // vertical size of view volume in world units
        ClipPlanes clip_planes{};
    };

}

#endif // GRAPHICS_CAMERA_ORTHOGRAPHIC_CAMERA_H
