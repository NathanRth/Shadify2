#include "scene.h"
#include <iostream>

#include <glm/gtx/string_cast.hpp>

Scene::Scene()
{
    m_camera = new CameraOrbit(100,100);
}

void Scene::addModel(const Model &model)
{
    m_models.push_back(model);
}


void Scene::addLight(Light *light)
{
    m_lights.push_back(light);
}


void Scene::removeModel(unsigned int index)
{
    m_models.erase(m_models.begin() += index);
}

void Scene::removeLight(unsigned int index)
{
    m_lights.erase(m_lights.begin() += index);
}



