#include <graphics/factories/texture_factories.h>

#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace graphics::factories
{

    std::expected<components::Texture, std::string> create_texture_from_file(const char* path)
    {
        int width = 0, height = 0, channels = 0;
        unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

        if (!data)
            return std::unexpected(std::format("Failed to load image '{}': {}", path, stbi_failure_reason()));

        GLuint tex = 0;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Sampler parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);

        return components::Texture{ .id = tex, .width = width, .height = height };
    }

}
