#ifndef GRAPHICS_FACTORIES_TEXTURE_FACTORIES_H
#define GRAPHICS_FACTORIES_TEXTURE_FACTORIES_H

#include <expected>
#include <string>

#include <graphics/components/texture.h>

namespace graphics::factories
{
	
    /// <summary>
    /// Loads an image file using stb_image, uploads it to an OpenGL 2D texture,
    /// configures sampler parameters, and returns a Texture component containing
    /// the OpenGL texture handle and image dimensions.
    ///
    /// This function:
    ///   • loads the file as 4-channel RGBA (forcing alpha if needed)
    ///   • creates a GL texture object
    ///   • uploads pixel data with glTexImage2D
    ///   • generates mipmaps
    ///   • sets linear filtering and repeat wrapping
    ///
    /// On success:
    ///   Returns Texture { id, width, height } with a valid GL texture handle.
    ///
    /// On failure:
    ///   Returns std::unexpected with an error message describing the issue.
    /// </summary>
    /// <param name="path">Filesystem path to the image file</param>
    /// <returns>
    /// std::expected containing:
    ///   • Texture on success
    ///   • std::string error message on failure
    /// </returns>
    std::expected<components::Texture, std::string> create_texture_from_file(const char* path);

}

#endif // GRAPHICS_FACTORIES_TEXTURE_FACTORIES_H
