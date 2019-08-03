#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

/**
 * @brief Class mère des objets caméra.
 * @warning Cette classe ne doit pas être instanciée
 */
class Camera
{
public:
    Camera(int width, int height);

    /**
     * @brief setFormat
     * @param height Hauteur du context OpenGL
     * @param width Largeur du context OpenGL
     */
    void setFormat(int height, int width);

    /**
     * @brief setViewAngle
     * @param angle Champ de vision (field of view) (en degrés)
     */
    void setViewAngle(float angle);

    /**
     * @brief Distance minimale du frustrum
     * @param dist
     */
    void setNearPlane(float dist);

    /**
     * @brief Distance maximale du frustrum
     * @param dist
     */
    void setFarPlane(float dist);

    /**
     * @brief Renvoie la matrice Caméra/View (Model - View - Projection)
     */
    glm::mat4 viewMatrix() {return m_view;}

    /**
     * @return Renvoie la matrice de projection
     */
    glm::mat4 projectionMatrix() {return m_projection;}

    /**
     * @brief Calcule la matrice de projection. Elle est calculée à chaque fois que le format, le viewAngle et le frustrum sont changés.
     */
    void computeProjection();

    /**
     * @brief Calcule la matrice Caméra/View. Elle est calculée à chaque déplacement de la caméra.
     */
    virtual void updateView() = 0;

protected:
    glm::mat4 m_view, m_projection;
    float m_nearPlane, m_farPlane;
    float m_viewAngle;
    int m_width, m_height;
};

#endif // CAMERA_H
