#ifndef GRAPHICS_COMPONENTS_TRANSFORM_H
#define GRAPHICS_COMPONENTS_TRANSFORM_H

#include <glm/glm.hpp>

namespace graphics::components
{

    struct Transform
    {
        Transform();
        Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl);

        auto get_position() const -> glm::vec3;
        auto set_position(const glm::vec3& pos) -> void;

        auto get_rotation() const -> glm::vec3;
        auto set_rotation(const glm::vec3& rot) -> void;

        auto get_scale() const -> glm::vec3;
        auto set_scale(const glm::vec3& scl) -> void;

        bool dirty{ false };

    private:
        glm::vec3 position  { 0.f, 0.f, 0.f };
        glm::vec3 rotation  { 0.f, 0.f, 0.f }; // Euler angles in radians
        glm::vec3 scale     { 1.f, 1.f, 1.f };
    };

}

#endif // GRAPHICS_COMPONENTS_TRANSFORM_H
