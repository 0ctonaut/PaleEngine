#pragma once
#include <filesystem>
#include "Core/PaleRendererExport.h"
#include "OpenGL/MeshOpenGL.h"

namespace PaleRenderer
{
	class PALE_API CModel
	{
	public:
		bool load(std::filesystem::path vPath);
	private:
		bool __processNode(aiNode*, const aiScene*);
		CMeshOpenGL __processMesh(aiMesh*, const aiScene*);
		std::vector<CTextureOpenGL> __loadTexturesFromMat(aiMaterial*, aiTextureType, ETexture);

		std::vector<CMeshOpenGL> m_Meshes;
		std::filesystem::path m_ModelPath;
	};

}