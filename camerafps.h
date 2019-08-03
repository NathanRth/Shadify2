#ifndef CAMERAFPS_H
#define CAMERAFPS_H

#include <glm.hpp>

#include "camera.h"

/**
 * @brief The CameraFPS class
 */
class CameraFPS : public Camera
{
public:
    CameraFPS(int width, int height);

    void strafLeft();
    void strafRight();
    void forward();
    void backward();
    void look(double dx, double dy);

    glm::vec3 position() {return m_pos;}

    void updateView() override;

private:
    glm::vec3 m_forward, m_up;
    glm::vec3 m_pos;
    float m_yaw, m_pitch;
    float m_speed;
};

#endif // CAMERAFPS_H
