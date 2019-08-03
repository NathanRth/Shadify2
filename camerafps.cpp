#include "camerafps.h"
#include "glm/gtc/matrix_transform.hpp"

CameraFPS::CameraFPS(int width, int height):
    Camera(width, height)
{
    m_forward = glm::vec3(0,0,1);
    m_up = glm::vec3(0,1,0);
    m_pos = glm::vec3(0,0,0);
    m_yaw = 0;
    m_pitch = 0;
    m_speed = 0.01;
}

void CameraFPS::strafLeft()
{
    m_pos -= glm::cross(m_forward,m_up) * m_speed;
    updateView();
}

void CameraFPS::strafRight()
{
    m_pos += glm::cross(m_forward,m_up) * m_speed;
    updateView();
}

void CameraFPS::forward()
{
    m_pos += m_forward * m_speed;
    updateView();
}

void CameraFPS::backward()
{
    m_pos -= m_forward * m_speed;
    updateView();
}

void CameraFPS::look(double dx, double dy)
{
    m_yaw += -dx*0.1;
    m_pitch += dy*0.1;

    m_forward.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    m_forward.y = sin(glm::radians(m_pitch));
    m_forward.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

    updateView();
}

void CameraFPS::updateView()
{
    m_view = glm::lookAt(m_pos, m_pos+m_forward, m_up);
}
