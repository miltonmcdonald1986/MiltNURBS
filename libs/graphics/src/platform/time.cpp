#include <graphics/platform/time.h>

#include <graphics/platform/gl_includes.h>

namespace graphics::platform
{

    void Time::update()
    {
        double now = glfwGetTime();

        if (prev == 0.)
            dt = 0.f;
        else
            dt = static_cast<float>(now - prev);

        prev = now;
    }

}
