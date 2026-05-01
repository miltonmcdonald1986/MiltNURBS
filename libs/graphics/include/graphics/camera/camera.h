#ifndef GRAPHICS_CAMERA_CAMERA_H
#define GRAPHICS_CAMERA_CAMERA_H

namespace graphics::camera
{

    struct Camera 
    {
        enum class ProjectionType { Perspective, Orthographic };
        ProjectionType type{ ProjectionType::Perspective };
    };

}

#endif // GRAPHICS_COMPONENTS_CAMERA_H
