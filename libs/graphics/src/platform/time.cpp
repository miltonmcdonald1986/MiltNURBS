#include <graphics/platform/time.h>

#include <graphics/platform/gl_includes.h>

namespace graphics::platform::time
{

    void Time::update()
    {
        double now = glfwGetTime();

        if (last_time == 0.)
            delta_time = 0.;
        else
            delta_time = now - last_time;

        last_time = now;
    }

}
