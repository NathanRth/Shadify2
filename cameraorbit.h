#ifndef CAMERAORBIT_H
#define CAMERAORBIT_H

#include <glm.hpp>
#include "camera.h"

/**
 * @brief Camera de type orbit.
 */
class CameraOrbit : public Camera
{
public:
    enum Axis{X,Y,Z};

    CameraOrbit(int width, int height);

    void setRotation(float x, float y, float z);
    glm::vec3 rotation() {return glm::vec3(m_rotX, m_rotY, m_rotZ);}
    void setPosition(float x, float y, float z);
    void setPosition(const glm::vec3 &pos);
    glm::vec3 position();

    void rotateBy(int dx, int dy);
    void rotateBy(float angle,Axis axis);
    void translateBy(int dx, int dy);
    void translateBy(const glm::vec3 &v);
    void setDist(int zoom);

    void updateView() override;

private:

    float m_posX,m_posY,m_posZ;
    float m_rotX,m_rotY,m_rotZ;
    float m_dist;

};

#endif // CAMERAORBIT_H
