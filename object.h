#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "shader.h"

/**
 * @brief The Object class
 * La classe Objet réprésente la matrice Model d'un objet dans la scène.
 */
class Object
{
public:
    Object();

    /**
     * @brief Déplace l'objet linéairement
     * @param t Vecteur de translation
     */
    void translate(const glm::vec3 &t);

    /**
     * @brief Tourne l'objet de \a angle degrés selon l'axe \a axis
     * @param angle L'angle de rotation
     * @param axis L'axe de rotation
     */
    void rotate(float angle, const glm::vec3 &axis);

    /**
     * @brief Scale l'objet selon une valeur définie pour chaque axe
     * @param fac Vecteur contenant les facteurs d'agrandissement
     */
    void scale(const glm::vec3 &fac);

    /**
     * @brief Scale l'objet sur tout les axe
     * @param fac Facteur d'agrandissement
     */
    void scale(const float &fac);

    /**
     * @brief Annule toutes les transformations de l'objet
     */
    void resetTransform();

    /**
     * @brief Défini la position de l'objet comme étant \a p
     * @param p Nouvelle position (absolue)
     */
    void setPosition(const glm::vec3 &p);

    glm::vec3 position();

    virtual void draw(Shader shader){}

    /**
     * @return Renvois la matrice Objet (Model - View - Projection)
     */
    glm::mat4 modelMatrix() {return m_modelMatrix;}

protected:

    glm::mat4 m_modelMatrix; /**< Matrice objet/model (Model - View - Projection) */
};

#endif // OBJECT_H
