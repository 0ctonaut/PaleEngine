#pragma once
#include <filesystem>
#include <unordered_map>
#include <assimp/scene.h>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/OpenGL/MeshOpenGL.h"
#include "PaleRenderer/OpenGL/PassOpenGL.h"

namespace PaleRdr
{
	class PALE_API IModelLoader
	{
	public:
		static std::vector<CMeshOpenGL> getCubeMeshes();
	public:
		virtual bool loadV(std::filesystem::path vPath) { return false; }
	protected:
		std::filesystem::path m_ModelPath;
		std::unordered_map<std::filesystem::path, CTextureOpenGL> m_TexCache;
	};

	class PALE_API CModelLoader : public IModelLoader
	{

	public:
		CModelLoader() = default;
		CModelLoader(std::filesystem::path vPath);
		virtual bool loadV(std::filesystem::path vPath) override;
		inline std::vector<CMeshOpenGL> getMeshes() { return m_Meshes; }

	private:
		bool __processNode(aiNode*, const aiScene*);
		CMeshOpenGL __processMesh(aiMesh*, const aiScene*);
		std::vector<CTextureOpenGL> __loadTexturesFromMat(aiMaterial*, aiTextureType, ETexture, bool vFlip = true);
		std::vector<CMeshOpenGL> m_Meshes;
	};

}