#pragma once
#include "libs.h"

/**
 * @brief A structure representing a vertex in a 3D model.
 */
struct Vertex {
    glm::vec3 Position; ///< The position of the vertex.
    glm::vec3 Normal; ///< The normal vector at the vertex.
    glm::vec2 TexCoords; ///< The texture coordinates at the vertex.
};
