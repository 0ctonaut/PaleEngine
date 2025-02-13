#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"
#include "PaleRenderer/OpenGL/PassOpenGL.h"

namespace PaleRdr
{
	class PALE_API CVertexOpenGL
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		glm::vec3 Tangent;
		glm::vec3 BiTangent;
	};

	class PALE_API CMeshOpenGL
	{
	public:
		CMeshOpenGL(
			const std::vector<CVertexOpenGL>& vVertices,
			const std::vector<unsigned int>& vIndices,
			bool vUseTex, bool vUseNormalMap
		);

		CMeshOpenGL(
			const std::vector<CVertexOpenGL>& vVertices,
			const std::vector<CTextureOpenGL>& vTextures,
			const std::vector<unsigned int>& vIndices,
			bool vUseTex, bool vUseNormalMap
		);

		//CMeshOpenGL(const CMeshOpenGL&) = delete;
		//CMeshOpenGL& operator=(const CMeshOpenGL&) = delete;

		//CMeshOpenGL(CMeshOpenGL&& vOther);
		~CMeshOpenGL();

		void draw(const CPassOpenGL& vPass);

	private:
		void __initMesh();

		std::vector<CVertexOpenGL> m_Vertices;
		std::vector<CTextureOpenGL> m_Textures;
		std::vector<unsigned int> m_Indices;
		unsigned int m_VAO, m_VBO, m_EBO;
		bool m_bUseTex;
		bool m_bUseNormalMap;
	};
}