#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "object.h"

#define POINT_LIGHT         0
#define DIRECTIONAL_LIGHT   1

/**
 * @brief The Light class
 * Classe abstraite des objets de type lumière. Cette classe porte
 * les propriétés communes des différents types de lumière.
 * @see \c PointLight
 * @see \c DirectionalLight
 */
class Light : public Object
{
public:

    /**
     * @brief The LightType enum
     * La valeur de \a type sera utilisé par GLSL pour déterminer le type de lumière.
     */
    enum LightType{
        PointLight = 0,
        DirectionalLight = 1
    };

    glm::vec3 color; /**< La couleur de la lumière */
    int type; /**< Le type de la lumière \see LightType */
    float intensity; /**< L'intensité de la lumière */
    glm::vec3 direction; /**< La direction de la lumière (seulement les DirectionalLight) */

};

/**
 * @brief The PointLight class
 */
class PointLight : public Light
{
public:
    PointLight(const glm::vec3 &c, const glm::vec3 &p, float i);
};

/**
 * @brief The DirectionalLight class
 */
class DirectionalLight : public Light
{
public:
    DirectionalLight(const glm::vec3 &c, const glm::vec3 &p, float i, const glm::vec3 &d);
};

#endif // LIGHT_H
