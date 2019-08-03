#include <iostream>

#include "cameraorbit.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"


CameraOrbit::CameraOrbit(int width, int height):
    Camera(width, height)
{
    m_rotX=3.0;
    m_rotY=-90;
    m_rotZ=0.0;
    m_posX=4.0;
    m_posY=1.54;
    m_posZ=0.30;
    m_dist=9;
    updateView();
}


void CameraOrbit::setPosition(float x, float y, float z)
{
    m_posX = x;
    m_posY = y;
    m_posZ = z;
    updateView();
}

void CameraOrbit::setPosition(const glm::vec3 &pos)
{
    m_posX = pos.x;
    m_posY = pos.y;
    m_posZ = pos.z;
    updateView();
}

void CameraOrbit::setRotation(float x, float y, float z)
{
    m_rotX = x;
    m_rotY = y;
    m_rotZ = z;
    updateView();
}

glm::vec3 CameraOrbit::position()
{
    float x = -m_posX + m_dist * -glm::cos(glm::radians(m_rotX)) * glm::sin(glm::radians(m_rotY));
    float y = m_posY + m_dist * glm::sin(glm::radians(m_rotX));
    float z = -m_posZ + m_dist * glm::cos(glm::radians(m_rotX)) * glm::cos(glm::radians(m_rotY));
    return glm::vec3(x,y,z);
}

void CameraOrbit::translateBy(int dx, int dy)
{
    dx *= m_dist;
    dy *= m_dist;
    float s = 20;
    m_posX += cos(glm::radians(m_rotY))*(float)(dx/s)/30.0;
    m_posZ += sin(glm::radians(m_rotY))*(float)(dx/s)/30.0;
    m_posY += cos(glm::radians(m_rotX))*(float)(dy/s)/30.0;
    updateView();
}

void CameraOrbit::translateBy(const glm::vec3 &v)
{
    m_posX += v.x;
    m_posY += v.y;
    m_posZ += v.z;
    updateView();
}

void CameraOrbit::rotateBy(int dx, int dy)
{
    m_rotX+=dy/5.0;
    m_rotY+=dx/5.0;
    updateView();
}

void CameraOrbit::rotateBy(float angle, Axis axis)
{
    switch (axis) {
    case X:
        m_rotX += angle;
        break;
    case Y:
        m_rotY += angle;
        break;
    case Z:
        m_rotZ += angle;
        break;
    default:
        break;
    }
    updateView();
}

void CameraOrbit::setDist(int zoom)
{
    m_dist *= 1.0f + (-1.0f *(float)(zoom) / 30.0f);
    updateView();
}

void CameraOrbit::updateView()
{
    //std::cout<<"dist: "<<m_dist<<" | pos: "<<m_posX<<" "<<m_posY<<" "<<m_posZ<<" | rot: "<<m_rotX<<" "<<m_rotY<<" "<<m_rotZ<<std::endl;
    m_view = glm::lookAt(glm::vec3(0, 0, m_dist),glm::vec3(0,0,0),glm::vec3(0,1,0));
    m_view = glm::rotate(m_view,glm::radians(m_rotX), glm::vec3(1.0f,0.0f,0.0f));
    m_view = glm::rotate(m_view,glm::radians(m_rotY), glm::vec3(0.0f,1.0f,0.0f));
    m_view = glm::translate(m_view,glm::vec3(m_posX,-m_posY,m_posZ));
}
