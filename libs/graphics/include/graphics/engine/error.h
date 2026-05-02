#ifndef GRAPHICS_ENGINE_ERROR_H
#define GRAPHICS_ENGINE_ERROR_H

#include <stacktrace>

#if defined(DEBUG) || defined(_DEBUG)
    #define CAPTURE_TRACE std::stacktrace::current()
#else
    #define CAPTURE_TRACE std::stacktrace{}
#endif

#define ERR(msg) \
    graphics::engine::ErrorInfo{ msg, NAMESPACE, __FILE__, __LINE__, CAPTURE_TRACE }

namespace graphics::engine
{

    struct ErrorInfo 
    {
        std::string message{};
        std::string category{};
        std::string file{};
        int line{ 0 };
        std::stacktrace trace{}; // empty unless captured
    };

    inline void log_error(const ErrorInfo& /*err*/)
    {
        //spdlog::error("[{}] {} ({}:{})",
        //    err.category, err.message, err.file, err.line);

        //if (!err.trace.empty()) {
        //    spdlog::error("Stacktrace:");
        //    for (auto& f : err.trace) {
        //        spdlog::error("  {}", f);
        //    }
        //}
    }

}

#endif // GRAPHICS_ENGINE_ERROR_H
