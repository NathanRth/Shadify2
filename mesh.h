#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>

#include <GL/gl.h>

#include <vector>

#include "object.h"

#include "vertex.h"
#include "texture.h"
#include "shader.h"

/**
 * @brief The Mesh class
 */
class Mesh : public Object
{
public:
    Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t);

    void draw(Shader shader);

private:

    void makeMesh();

    unsigned int m_vao, m_vbo, m_ebo;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;


};

#endif // MESH_H
