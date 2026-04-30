#ifndef GRAPHICS_COMPONENTS_TEXTURE_H
#define GRAPHICS_COMPONENTS_TEXTURE_H

#include <graphics/platform/gl_includes.h>

namespace graphics::components::texture
{

    struct Texture {
        GLuint id = 0;
        int width = 0;
        int height = 0;
    };
	
}

#endif // GRAPHICS_COMPONENTS_TEXTURE_H
