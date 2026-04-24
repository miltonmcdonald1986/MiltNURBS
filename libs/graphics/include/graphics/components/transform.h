#ifndef GRAPHICS_COMPONENTS_TRANSFORM_H
#define GRAPHICS_COMPONENTS_TRANSFORM_H

#include <glm/glm.hpp>

namespace graphics::components::transform
{

    struct Transform
    {
        bool dirty{ false };

        glm::vec3 position  { 0.f, 0.f, 0.f };
        glm::vec3 rotation  { 0.f, 0.f, 0.f }; // Euler angles in radians
        glm::vec3 scale     { 1.f, 1.f, 1.f };
    };

}

#endif // GRAPHICS_COMPONENTS_TRANSFORM_H
