#include "stdafx.h"
#include "PaleRenderer/Mesh/Cube.h"

namespace PaleRdr
{
    std::vector<std::shared_ptr<IMesh>> Cube::getMeshes()
    {
        if (!m_bInit)
        {
            __calculateCube();
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

    void Cube::__calculateCube()
    {
        m_Position.clear();
        m_Normal.clear();
        m_TexCoord.clear();
        m_Indices.clear();

        auto addFace = [](const glm::vec3& bl, const glm::vec3& tl,
            const glm::vec3& br, const glm::vec3& tr,
            const glm::vec3& faceNormal)
            {
                if (!m_Indices.empty())
                {
                    m_Indices.push_back(m_Indices.back());
                }
                unsigned int startIndex = m_Position.size();

                m_Position.push_back(bl);
                m_Position.push_back(tl);
                m_Position.push_back(br);
                m_Position.push_back(tr);

                m_TexCoord.push_back(glm::vec2(0.0f, 0.0f));
                m_TexCoord.push_back(glm::vec2(0.0f, 1.0f));
                m_TexCoord.push_back(glm::vec2(1.0f, 0.0f));
                m_TexCoord.push_back(glm::vec2(1.0f, 1.0f));

                m_Normal.push_back(faceNormal);
                m_Normal.push_back(faceNormal);
                m_Normal.push_back(faceNormal);
                m_Normal.push_back(faceNormal);

                m_Indices.push_back(startIndex + 0); // bl
                m_Indices.push_back(startIndex + 1); // tl
                m_Indices.push_back(startIndex + 2); // br
                m_Indices.push_back(startIndex + 3); // tr
            };

        // Front face (z = +1)
        addFace(glm::vec3(-1.0f, -1.0f, 1.0f),  // bl
            glm::vec3(-1.0f, 1.0f, 1.0f),  // tl
            glm::vec3(1.0f, -1.0f, 1.0f),  // br
            glm::vec3(1.0f, 1.0f, 1.0f),  // tr
            glm::vec3(0.0f, 0.0f, 1.0f));

        // Back face (z = -1)
        addFace(glm::vec3(1.0f, -1.0f, -1.0f),  // bl
            glm::vec3(1.0f, 1.0f, -1.0f),  // tl
            glm::vec3(-1.0f, -1.0f, -1.0f),  // br
            glm::vec3(-1.0f, 1.0f, -1.0f),  // tr
            glm::vec3(0.0f, 0.0f, -1.0f));

        // Right face (x = +1)
        addFace(glm::vec3(1.0f, -1.0f, 1.0f),  // bl
            glm::vec3(1.0f, 1.0f, 1.0f),  // tl
            glm::vec3(1.0f, -1.0f, -1.0f),  // br
            glm::vec3(1.0f, 1.0f, -1.0f),  // tr
            glm::vec3(1.0f, 0.0f, 0.0f));

        // Left face (x = -1)
        addFace(glm::vec3(-1.0f, -1.0f, -1.0f),  // bl
            glm::vec3(-1.0f, 1.0f, -1.0f),  // tl
            glm::vec3(-1.0f, -1.0f, 1.0f),  // br
            glm::vec3(-1.0f, 1.0f, 1.0f),  // tr
            glm::vec3(-1.0f, 0.0f, 0.0f));

        // Top face (y = +1)
        addFace(glm::vec3(-1.0f, 1.0f, 1.0f),  // bl
            glm::vec3(-1.0f, 1.0f, -1.0f),  // tl
            glm::vec3(1.0f, 1.0f, 1.0f),  // br
            glm::vec3(1.0f, 1.0f, -1.0f),  // tr
            glm::vec3(0.0f, 1.0f, 0.0f));

        // Bottom face (y = -1)
        addFace(glm::vec3(-1.0f, -1.0f, -1.0f),  // bl
            glm::vec3(-1.0f, -1.0f, 1.0f),  // tl
            glm::vec3(1.0f, -1.0f, -1.0f),  // br
            glm::vec3(1.0f, -1.0f, 1.0f),  // tr
            glm::vec3(0.0f, -1.0f, 0.0f));
    }
}
