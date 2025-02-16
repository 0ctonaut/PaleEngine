#include "stdafx.h"
#include "PaleRenderer/OpenGL/MeshOpenGL.h"

namespace PaleRdr
{
    CMeshOpenGL::CMeshOpenGL(
        const std::vector<CVertex>& vVertices, 
        const std::vector<unsigned int>& vIndices,
        unsigned int vMaterialIndex,
        unsigned int vElementType) 
        : m_MaterialIndex(vMaterialIndex), m_ElementType(vElementType)
    {
        m_Vertices = vVertices;
        m_Indices = vIndices;
        __initMesh();
    }

    CMeshOpenGL::~CMeshOpenGL()
    {
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

	void CMeshOpenGL::__initMesh()
	{
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(CVertex), &m_Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
            &m_Indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, Normal));
        // vertex texture coords

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, TexCoord));


        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)offsetof(CVertex, BiTangent));
        
        glBindVertexArray(0);
	}
}
