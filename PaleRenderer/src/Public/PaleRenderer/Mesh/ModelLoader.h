#pragma once
#include <filesystem>
#include <unordered_map>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/Mesh/Mesh.h"
#include "PaleRenderer/Material/Texture.h"
#include "PaleRenderer/Material/Material.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace PaleRdr
{
	class PALE_API IModelLoader
	{
	public:
		virtual bool loadV(std::filesystem::path vPath) { return false; }
	protected:
		std::filesystem::path m_ModelPath;
		std::unordered_map<std::filesystem::path, std::shared_ptr<ITexture>> m_TexCache;
	};

	class PALE_API CModelLoader : public IModelLoader
	{
	public:
		CModelLoader() = default;
		CModelLoader(std::filesystem::path vPath);
		virtual bool loadV(std::filesystem::path vPath) override;
		inline std::vector<std::shared_ptr<IMesh>> getMeshes() { return m_Meshes; }
		inline std::unordered_map<int, std::shared_ptr<IMaterial>> getMaterials() { return m_Materials; }

	private:
		bool __processNode(aiNode*, const aiScene*);
		void __processMesh(aiMesh*, const aiScene*);
		std::vector<std::shared_ptr<ITexture>> __loadTexturesFromMat(aiMaterial*, int, ETexture);
		std::vector<std::shared_ptr<IMesh>> m_Meshes;
		std::unordered_map<int, std::shared_ptr<IMaterial>> m_Materials;
	};

}