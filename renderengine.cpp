#include "renderengine.h"

#define SCR_WIDTH   1280
#define SCR_HEIGHT  720

std::default_random_engine RenderEngine::generator(std::random_device{}());
std::uniform_real_distribution<GLfloat> RenderEngine::randomFloats{0.0, 1.0};
unsigned int RenderEngine::quadVAO = 0;
unsigned int RenderEngine::quadVBO;

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}
RenderEngine::RenderEngine()
{

}

std::vector<glm::vec3> RenderEngine::genSampleKernel(unsigned int nb)
{

    std::vector<glm::vec3> kernel;
    for (unsigned int i = 0; i < nb; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator)*2.0-1.0, randomFloats(generator));

        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / float(nb);

        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        kernel.push_back(sample);
    }
    return kernel;
}

GLuint RenderEngine::genNoiseTexture(unsigned int size)
{
    // generate noise texture
    std::vector<glm::vec3> noiseTex;
    for (unsigned int i = 0; i < size*size; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        noiseTex.push_back(noise);
    }
    unsigned int noiseTexture;
    glGenTextures(1,&noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, &noiseTex[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return noiseTexture;
}

void RenderEngine::renderBuffer()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

GLuint RenderEngine::loadCubemap(const std::string &dir)
{
    GLuint textureID;
    int width, height, nrChannels;
    unsigned char *data;

    std::vector<std::string> textures_faces = {
        dir+"right.jpg",
        dir+"left.jpg",
        dir+"top.jpg",
        dir+"bottom.jpg",
        dir+"front.jpg",
        dir+"back.jpg"
    };

    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for(GLuint i = 0; i < textures_faces.size(); i++)
    {
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        //std::cout<<textures_faces[i].c_str()<<" "<<i<<" ";
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
