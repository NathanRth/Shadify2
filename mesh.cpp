#include "mesh.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t):
    Object()
{
    m_vertices = v;
    m_indices = i;
    m_textures = t;
    makeMesh();
}

void Mesh::draw(Shader shader)
{
    shader.bind();

    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    bool hasDiffuse = false;
    bool hasSpecular = false;
    bool hasNormal = false;
    bool hasHeight = false;
    for(unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = m_textures[i].type;
        if(name == "texture_diffuse"){
            number = std::to_string(diffuseNr++);
            hasDiffuse = true;
        }
        else if(name == "texture_specular"){
            number = std::to_string(specularNr++); // transfer unsigned int to stream
            hasSpecular = true;
        }
        else if(name == "texture_normal"){
            number = std::to_string(normalNr++); // transfer unsigned int to stream
            hasNormal = true;
        }
        else if(name == "texture_height"){
            number = std::to_string(heightNr++); // transfer unsigned int to stream
            hasHeight = true;
        }

        // now set the sampler to the correct texture unit
        shader.setUniformInt((name + number).c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    shader.setUniformBool("hasDiffuse", hasDiffuse);
    shader.setUniformBool("hasSpecular", hasSpecular);
    shader.setUniformBool("hasNormal", hasNormal);
    shader.setUniformBool("hasHeight", hasHeight);
    glActiveTexture(GL_TEXTURE0);

    // draw mesh

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::makeMesh()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
                 &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}
