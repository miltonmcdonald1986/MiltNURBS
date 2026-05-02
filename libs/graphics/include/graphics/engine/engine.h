#ifndef GRAPHICS_ENGINE_ENGINE_H
#define GRAPHICS_ENGINE_ENGINE_H

#include <expected>
#include <functional>
#include <string>

#include <graphics/engine/result.h>

namespace graphics::engine { struct AppData; }

namespace graphics::engine
{

	using InitFn = std::function<Status(engine::AppData*)>;
	using UpdateFn = std::function<Status(engine::AppData*)>;

	Status run(InitFn init_fn, UpdateFn update_fn);

}

#endif // GRAPHICS_ENGINE_ENGINE_H
