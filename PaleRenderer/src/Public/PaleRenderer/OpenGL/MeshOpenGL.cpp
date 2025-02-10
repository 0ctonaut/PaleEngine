#include "stdafx.h"
#include "PaleRenderer/OpenGL/MeshOpenGL.h"

namespace PaleRdr
{
	CMeshOpenGL::CMeshOpenGL(const std::vector<CVertexOpenGL>& vVertices, const std::vector<CTextureOpenGL>& vTextures, const std::vector<unsigned int>& vIndices)
	{
		m_Vertices = vVertices;
		m_Textures = vTextures;
		m_Indices = vIndices;
        __initMesh();
	}

	CMeshOpenGL::CMeshOpenGL(std::vector<CVertexOpenGL>&& vVertices, std::vector<CTextureOpenGL>&& vTextures, std::vector<unsigned int>&& vIndices)
	{
		m_Vertices = vVertices;
		m_Textures = vTextures;
		m_Indices = vIndices;
        __initMesh();
	}

    void CMeshOpenGL::draw(const CPassOpenGL& vPass)
    {
        vPass.use();
        for (unsigned int i = 0; i < m_Textures.size(); ++i)
        {
            unsigned int numDiffuse = 1;
            unsigned int numSpecular = 1;
            glActiveTexture(GL_TEXTURE0 + i);
            std::string name;
            if (m_Textures[i].Type == ETexture::Diffuse)
            {
                name = std::format("mat.tex_diffuse{}", numDiffuse++);
            }
            else if (m_Textures[i].Type == ETexture::Specular)
            {
                name = std::format("mat.tex_specular{}", numSpecular++);
            }
            else
            {
                spdlog::error("Unknown texture type.");
            }

            vPass.setInt(name, i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].getTexID());
        }
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
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CVertexOpenGL), (void*)offsetof(CVertexOpenGL, TexCoord));

        glBindVertexArray(0);
	}
}
