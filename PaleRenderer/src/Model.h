#pragma once
#include <filesystem>
#include <unordered_map>
#include <assimp/scene.h>
#include "Core/PaleRendererExport.h"
#include "OpenGL/MeshOpenGL.h"
#include "OpenGL/PassOpenGL.h"

namespace PaleRenderer
{
	class PALE_API CModel
	{
	public:
		CModel() = default;
		CModel(std::filesystem::path vPath);
		bool load(std::filesystem::path vPath);
		void draw(CPassOpenGL& vPass);
	private:
		bool __processNode(aiNode*, const aiScene*);
		CMeshOpenGL __processMesh(aiMesh*, const aiScene*);
		std::vector<CTextureOpenGL> __loadTexturesFromMat(aiMaterial*, aiTextureType, ETexture);

		std::vector<CMeshOpenGL> m_Meshes;
		std::filesystem::path m_ModelPath;

		std::unordered_map<std::filesystem::path, CTextureOpenGL> m_TexCache;
	};

}