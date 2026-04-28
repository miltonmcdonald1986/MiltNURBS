#ifndef GRAPHICS_COMPONENTS_TRANSFORM_H
#define GRAPHICS_COMPONENTS_TRANSFORM_H

#include <glm/glm.hpp>

namespace graphics::components::transform
{

    struct Transform
    {
        bool dirty{ false };

        glm::vec3 get_position() const
        {
            return position;
        }

        void set_position(const glm::vec3& pos)
        {
            position = pos;
            dirty = true;
        }

        glm::vec3 get_rotation() const
        {
            return rotation;
        }

        void set_rotation(const glm::vec3& rot)
        {
            rotation = rot;
            dirty = true;
        }

        glm::vec3 get_scale() const
        {
            return scale;
        }

        void set_scale(const glm::vec3& scl)
        {
            scale = scl;
            dirty = true;
        }

    private:
        glm::vec3 position  { 0.f, 0.f, 0.f };
        glm::vec3 rotation  { 0.f, 0.f, 0.f }; // Euler angles in radians
        glm::vec3 scale     { 1.f, 1.f, 1.f };
    };

}

#endif // GRAPHICS_COMPONENTS_TRANSFORM_H
