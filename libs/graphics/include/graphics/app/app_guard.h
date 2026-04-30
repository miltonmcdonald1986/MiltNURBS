#ifndef GRAPHICS_APP_APP_GUARD_H
#define GRAPHICS_APP_APP_GUARD_H

#include "app.h"

namespace graphics::app::app_guard
{
	
    /// <summary>
    /// RAII guard that holds a reference to an App and calls Shutdown(app) when destroyed.
    /// </summary>
    struct AppGuard
    {
        app::App& app;

        AppGuard(app::App& a);
        ~AppGuard();
    };

}

#endif // GRAPHICS_APP_APP_GUARD_H
