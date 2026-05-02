#ifndef GRAPHICS_ENGINE_RESULT_H
#define GRAPHICS_ENGINE_RESULT_H

#include <expected>

#include <graphics/engine/error.h>

namespace graphics::engine
{

	template<typename T>
	using Result = std::expected<T, ErrorInfo>;

	using Status = std::expected<void, ErrorInfo>;

}

#endif // GRAPHICS_ENGINE_RESULT_H
