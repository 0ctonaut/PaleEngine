#pragma once
#include <vector>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/Mesh/Mesh.h"

namespace PaleRdr
{
	class PALE_API CMeshOpenGL : public IMesh
	{
	public:
		CMeshOpenGL(
			const std::vector<CVertex>& vVertices,
			const std::vector<unsigned int>& vIndices,
			unsigned int vMaterialIndex,
			unsigned int vElementType
		);

		~CMeshOpenGL();

		inline unsigned int getMaterialIndex() const { return m_MaterialIndex; }
		inline unsigned int getVAO() const { return m_VAO; }
		inline unsigned int getElementType() const { return m_ElementType; }
		inline unsigned int getIndiceSize() const { return m_Indices.size(); }

	private:
		void __initMesh();

		std::vector<CVertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		unsigned int m_MaterialIndex;
		unsigned int m_VAO, m_VBO, m_EBO;
		unsigned int m_ElementType;
	};
}