#ifndef GRAPHICS_CAMERA_PERSPECTIVE_CAMERA_H
#define GRAPHICS_CAMERA_PERSPECTIVE_CAMERA_H

#include <graphics/camera/clip_planes.h>

namespace graphics::camera
{

    struct PerspectiveCamera
    {
        float fov{ 60.f }; // vertical field of view in degrees
        ClipPlanes clip_planes{};
    };

}

#endif // GRAPHICS_CAMERA_PERSPECTIVE_CAMERA_H
