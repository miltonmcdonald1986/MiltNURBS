#ifndef GRAPHICS_COMPONENTS_SHAKE_H
#define GRAPHICS_COMPONENTS_SHAKE_H

#include <glm/glm.hpp>

namespace graphics::components::shake
{

    struct Shake
    {
        double t = 0.0;
        float intensity = 0.02f; // how strong the shake is
        float speed = 60.0f;     // how fast it jitters
		
        glm::vec3 base_position{ 0.f }; // the anchor, should be set to the entity's original position and never modified by hand after that
    };

    struct ShakeOnce
    {
        float duration = 0.15f;   // total shake time
        float time_left = 0.15f;  // countdown
        float intensity = 0.05f;  // shake strength
        float speed = 60.0f;      // jitter speed
        
		glm::vec3 base_position{ 0.f };  // the anchor, should be set to the entity's original position and never modified by hand after that
    };

}

#endif // GRAPHICS_COMPONENTS_SHAKE_H
