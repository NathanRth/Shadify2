#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <random>
#include <map>

#include "shader.h"
#include "scene.h"

#include "stb_image/stb_image.h"

/**
 * @brief Class static contenant des fonctions utiles au rendu.
 * Cette class était initialement prévue pour acceuilir la totalité
 * des fonctions relatives au pipeline de rendu.
 */
class RenderEngine
{
public:
    RenderEngine();

    /**
     * @brief Génère un kernel de samples 3D.
     * \a nb samples sont générés aléatoirement dans un espace hemisphérique en
     * +y. Les samples sont également plus concentrés vers le centre.
     * @param nb Le nombre de samples à générer
     * @return La liste des points du kernel
     */
    static std::vector<glm::vec3> genSampleKernel(unsigned int nb);

    /**
     * @brief Génère une texture de bruit. Cette texture est notament utilisée pour
     * appliquer une rotation aléatoire au kernel afin de randomiser l'aspect du
     * SSAO ou du SSDO. La texture est d'une taille \a size * \a size
     * @param size Taille de la texture (en texel)
     * @return L'indice mémoire OpenGL de la texture
     */
    static GLuint genNoiseTexture(unsigned int size);

    /**
     * @brief Lance le rendu d'un pass avec le shader actif.
     */
    static void renderBuffer();

    /**
     * @brief loadCubemap
     * @param Chemin du dossier ou se trouve les fichiers de la cubemap.
     * @return L'indice mémoire OpenGL de la texture
     */
    static GLuint loadCubemap(const std::string &dir);

private:

    static std::default_random_engine generator;
    static std::uniform_real_distribution<GLfloat> randomFloats;

    static unsigned int quadVAO; /**< Vertex array du quad servant aux rendu des pass avec renderBuffer. */
    static unsigned int quadVBO; /**< Vertex buffer du quad */

};

#endif // RENDERENGINE_H
