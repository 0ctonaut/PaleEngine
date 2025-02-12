#pragma once
#include <glm/glm.hpp>
namespace PaleRdr
{
    struct SCompPointLight
    {
        SCompPointLight(const glm::vec3& vColor, float vIntensity)
            : _Color(vColor), _Intensity(vIntensity) {};
        glm::vec3 _Color;
        float _Intensity;
    };
}