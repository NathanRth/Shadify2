#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

typedef struct s_vertex
{
    glm::vec3 position; /**< Position du vertex */
    glm::vec3 normal; /**< Normal du vertex */
    glm::vec2 uv; /**< Position UV du vertex */
    glm::vec3 tangent; /**< Tengente du vertex, calculée par Assimp lors de l'importation */
} Vertex;

#endif // VERTEX_H
