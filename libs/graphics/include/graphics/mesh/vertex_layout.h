#ifndef GRAPHICS_MESH_VERTEX_LAYOUT_H
#define GRAPHICS_MESH_VERTEX_LAYOUT_H

#include <vector>

#include "vertex_attribute.h"

namespace graphics::mesh
{

    struct VertexLayout
    {
        size_t stride;  // bytes per vertex
        std::vector<VertexAttribute> attributes;    // list of attributes
    };

}

#endif // GRAPHICS_MESH_VERTEX_LAYOUT_H
