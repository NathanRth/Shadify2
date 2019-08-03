#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(int width, int height)
{
    m_width = width;
    m_height = height;

    m_view = glm::mat4();
    m_viewAngle = 45.0;
    m_nearPlane = 0.1f;
    m_farPlane = 100.0f;

}

void Camera::setViewAngle(float angle)
{
    if(angle <= 0.1)
        angle = 0.1;
    else if(angle >180)
        angle = 180;
    else
        m_viewAngle = angle;
    computeProjection();
}

void Camera::setNearPlane(float dist)
{
    if(dist < 0)
        m_nearPlane = 0;
    else if (dist > m_farPlane)
        m_nearPlane = m_farPlane;
    else
        m_nearPlane = dist;
    computeProjection();
}

void Camera::setFarPlane(float dist)
{
    if (dist < m_nearPlane)
        m_farPlane = m_nearPlane;
    else
        m_farPlane = dist;
    computeProjection();
}

void Camera::setFormat(int height, int width)
{
    m_height = height;
    m_width = width;
    computeProjection();
}

void Camera::computeProjection()
{
    m_projection = glm::perspective(glm::radians(m_viewAngle),(float)m_width/(float)m_height, m_nearPlane, m_farPlane);
}

