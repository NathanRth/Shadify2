#include "light.h"

PointLight::PointLight(const glm::vec3 &c, const glm::vec3 & p, float i)
{
    color = c;
    intensity = i;
    type = LightType::PointLight;
    direction = {0,0,0};
    translate(p);
}

DirectionalLight::DirectionalLight(const glm::vec3 &c, const glm::vec3 &p, float i, const glm::vec3 &d)
{
    color = c;
    intensity = i;
    type = LightType::DirectionalLight;
    direction = d;
    translate(p);
}
