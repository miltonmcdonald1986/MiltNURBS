#include <graphics/components/transform.h>

namespace graphics::components
{

    Transform::Transform()
        : position(glm::vec3(1.f))
        , rotation(glm::vec3(1.f))
        , scale(glm::vec3(1.f))
    {
    }

    Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl)
        : position(pos)
        , rotation(rot)
        , scale(scl)
    {
    }

    glm::vec3 Transform::get_position() const
    {
        return position;
    }

    void Transform::set_position(const glm::vec3& pos)
    {
        position = pos;
        dirty = true;
    }

    glm::vec3 Transform::get_rotation() const
    {
        return rotation;
    }

    void Transform::set_rotation(const glm::vec3& rot)
    {
        rotation = rot;
        dirty = true;
    }

    glm::vec3 Transform::get_scale() const
    {
        return scale;
    }

    void Transform::set_scale(const glm::vec3& scl)
    {
        scale = scl;
        dirty = true;
    }

}
