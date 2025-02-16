#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/Mesh/Mesh.h"

namespace PaleRdr
{
    class PALE_API Sphere
    {
    public:
        static std::vector<std::shared_ptr<IMesh>> getMeshes();

    private:
        static void __calculateSphere();
    private:
        inline static std::vector<glm::vec3> m_Position;
        inline static std::vector<glm::vec2> m_TexCoord;
        inline static std::vector<glm::vec3> m_Normal;
        inline static std::vector<unsigned int> m_Indices;

        inline static bool m_bInit = false;
    };
}
