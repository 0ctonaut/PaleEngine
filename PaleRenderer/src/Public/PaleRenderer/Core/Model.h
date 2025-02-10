#pragma once
#include <filesystem>
#include <unordered_map>
#include <assimp/scene.h>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/OpenGL/MeshOpenGL.h"
#include "PaleRenderer/OpenGL/PassOpenGL.h"

namespace PaleRdr
{
	class PALE_API CModel
	{
	public:
		CModel() = default;
		CModel(std::filesystem::path vPath);
		bool load(std::filesystem::path vPath);
		inline std::vector<CMeshOpenGL> getMeshes() { return m_Meshes; }

	private:
		bool __processNode(aiNode*, const aiScene*);
		CMeshOpenGL __processMesh(aiMesh*, const aiScene*);
		std::vector<CTextureOpenGL> __loadTexturesFromMat(aiMaterial*, aiTextureType, ETexture);

		std::vector<CMeshOpenGL> m_Meshes;
		std::filesystem::path m_ModelPath;

		std::unordered_map<std::filesystem::path, CTextureOpenGL> m_TexCache;
	};

}