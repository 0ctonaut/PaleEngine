#include "stdafx.h"
#include "PaleRenderer/OpenGL/MeshOpenGL.h"

namespace PaleRdr
{
    CMeshOpenGL::CMeshOpenGL(
        const std::vector<CVertexOpenGL>& vVertices, 
        const std::vector<unsigned int>& vIndices,
        bool vUseTex, bool vUseNormalMap) : m_bUseTex(vUseTex), m_bUseNormalMap(vUseNormalMap)
    {
        m_Vertices = vVertices;
        m_Indices = vIndices;
        __initMesh();
    }

	CMeshOpenGL::CMeshOpenGL(
        const std::vector<CVertexOpenGL>& vVertices, 
        const std::vector<CTextureOpenGL>& vTextures, 
        const std::vector<unsigned int>& vIndices,
        bool vUseTex, bool vUseNormalMap): m_bUseTex(vUseTex), m_bUseNormalMap(vUseNormalMap)
	{
		m_Vertices = vVertices;
		m_Textures = vTextures;
		m_Indices = vIndices;
        __initMesh();
	}

    //CMeshOpenGL::CMeshOpenGL(CMeshOpenGL&& vOther):
    //    m_EBO(vOther.m_EBO), m_VAO(vOther.m_VAO), m_VBO(vOther.m_VAO)
    //{
    //    m_EBO = 0;
    //    m_VAO = 0;
    //    m_VBO = 0;
    //}

    CMeshOpenGL::~CMeshOpenGL()
    {
        //glDeleteBuffers(1, &m_VBO);
        //glDeleteBuffers(1, &m_EBO);
        //glDeleteVertexArrays(1, &m_VAO);
    }

    void CMeshOpenGL::draw(const CPassOpenGL& vPass)
    {
        vPass.use();
        unsigned int numDiffuse = 1;
        unsigned int numSpecular = 1;
        unsigned int numNormal = 1;
        for (unsigned int i = 0; i < m_Textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string name;
            if (m_Textures[i].Type == ETexture::Diffuse)
            {
                name = std::format("tex_diffuse{}", numDiffuse++);
            }
            else if (m_Textures[i].Type == ETexture::Specular)
            {
                name = std::format("tex_specular{}", numSpecular++);
            }
            else if (m_Textures[i].Type == ETexture::Normal)
            {
                name = std::format("tex_normal{}", numNormal++);
            }
            else
            {
                spdlog::error("Unknown texture type.");
            }

            vPass.setInt(name, i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].getTexID());
        }

        vPass.setBool("uUseTex", m_bUseTex);
        vPass.setBool("uUseNormalMap", m_bUseTex && m_bUseNormalMap);

        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    }

	void CMeshOpenGL::__initMesh()
	{
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(CVertexOpenGL), &m_Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
            &m_Indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexOpenGL), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexOpenGL), (void*)offsetof(CVertexOpenGL, Normal));
        // vertex texture coords
        if (m_bUseTex)
        {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CVertexOpenGL), (void*)offsetof(CVertexOpenGL, TexCoord));
        }

        if (m_bUseTex && m_bUseNormalMap)
        {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexOpenGL), (void*)offsetof(CVertexOpenGL, Tangent));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexOpenGL), (void*)offsetof(CVertexOpenGL, BiTangent));
        }
        glBindVertexArray(0);
	}
}
