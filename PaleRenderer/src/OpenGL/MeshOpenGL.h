#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Core/PaleRendererExport.h"
#include "OpenGL/TextureOpenGL.h"
#include "OpenGL/PassOpenGL.h"

namespace PaleRenderer
{
	class PALE_API CVertexOpenGL
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	class PALE_API CMeshOpenGL
	{
	public:
		CMeshOpenGL(
			const std::vector<CVertexOpenGL>& vVertices,
			const std::vector<CTextureOpenGL>& vTextures,
			const std::vector<unsigned int>& vIndices
		);
		CMeshOpenGL(
			std::vector<CVertexOpenGL>&& vVertices, 
			std::vector<CTextureOpenGL>&& vTextures,
			std::vector<unsigned int>&& vIndices
		);

		void draw(const CPassOpenGL& vPass);

	private:
		void __initMesh();

		std::vector<CVertexOpenGL> m_Vertices;
		std::vector<CTextureOpenGL> m_Textures;
		std::vector<unsigned int> m_Indices;
		unsigned int m_VAO, m_VBO, m_EBO;
	};
}