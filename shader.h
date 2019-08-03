#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

/**
 * @brief Absraction des shader OpenGL.
 */
class Shader
{
public:
    Shader(std::string vertexPath, std::string fragmentPath);

    /**
     * @brief Active le shader pour le rendu
     */
    void bind();

    /**
     * @brief Determine l'emplacement des sampler pour le binding
     * @param samplers La liste des samplers
     */
    void setSamplers(std::vector<std::string> samplers);

    /**
     * @defgroup UniformSetters Uniform setters abstraction
     * @{
     */
    void setUniformFloat(std::string name, float val); /** Défini un float */
    void setUniformInt(std::string name, int val); /** Défini un entier */
    void setUniformUInt(std::string name, unsigned int val); /**< défini un entier non-signé */
    void setUniformBool(std::string name, bool val); /** Défini un booléen (cast vers un entier)*/
    void setUniformVec2(std::string name, glm::vec2 val); /** Défini un vecteur 2D */
    void setUniformVec3(std::string name, glm::vec3 val); /** Défini un vecteur 3D */
    void setUniformMat3(std::string name, glm::mat3 val); /** Défini une matrice 3x3 */
    void setUniformVec4(std::string name, glm::vec4 val); /** Défini un vecteur 4D */
    void setUniformMat4(std::string name, glm::mat4 val); /** Défini une matrice 4x4 */
    /**
      * @}
      */

private:
    unsigned int m_id;
    std::string vs; /**< Le chemin du vertex shader */
    std::string fs; /**< Le chemin du fragment shader */
    unsigned int m_nbSampler; /**< Le nombre de sampler du shader */

};

#endif // SHADER_H
