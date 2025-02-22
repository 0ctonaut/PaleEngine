#include "stdafx.h"
#include "PaleRenderer/Mesh/Quad.h"

namespace PaleRdr
{
    std::vector<std::shared_ptr<IMesh>> Quad::getMeshes()
    {
        if (!m_bInit)
        {
            __calculateQuad();
            m_bInit = true;
        }
        std::vector<std::shared_ptr<IMesh>> meshes;
        std::vector<CVertex> vertices;
        for (unsigned int i = 0; i < m_Position.size(); ++i)
        {
            CVertex vertex = { m_Position[i], m_Normal[i], m_TexCoord[i], glm::vec3(0.0f), glm::vec3(0.0f) };
            vertices.push_back(vertex);
        }

        meshes.push_back(IMesh::Create(vertices, m_Indices, 0, GL_TRIANGLE_STRIP));
        return meshes;
    }

    void Quad::__calculateQuad()
    {
        m_Position.clear();
        m_Normal.clear();
        m_TexCoord.clear();
        m_Indices.clear();

        m_Position = {
            glm::vec3(-1.0f,  1.0f, 0.0f),
            glm::vec3(-1.0f, -1.0f, 0.0f),
            glm::vec3(1.0f,  1.0f, 0.0f),
            glm::vec3(1.0f, -1.0f, 0.0f)
        };

        m_Normal = {
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };

        m_TexCoord = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f)
        };

        m_Indices = {
            0, 1, 2, 3
        };
    }
}
