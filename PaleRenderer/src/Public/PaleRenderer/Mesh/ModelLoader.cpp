#include "stdafx.h"
#include "PaleRenderer/Mesh/ModelLoader.h"

namespace PaleRdr
{
	CModelLoader::CModelLoader(std::filesystem::path vPath)
	{
		loadV(vPath);
	}
	bool CModelLoader::loadV(std::filesystem::path vPath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			vPath.string(), 
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			spdlog::error("ASSIMP {}", importer.GetErrorString());
			return false;
		}
		m_ModelPath = vPath.parent_path();
		if (__processNode(scene->mRootNode, scene))
		{
			return false;
		}
		return true;
	}

	bool CModelLoader::__processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			__processMesh(mesh, scene);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			if (!__processNode(node->mChildren[i], scene))
			{
				return false;
			}
		}
		return true;
	}

	void CModelLoader::__processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<CVertex> vertices;
		std::vector<unsigned int> indices;

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			CVertex vertex;
			glm::vec3 position(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);
			vertex.Position = position;

			glm::vec3 normal(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
			vertex.Normal = normal;

			glm::vec2 texcoord(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
			vertex.TexCoord = texcoord;
			
			glm::vec3 tangent(
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z
			);

			vertex.Tangent = tangent;

			glm::vec3 bitangent(
				mesh->mBitangents[i].x,
				mesh->mBitangents[i].y,
				mesh->mBitangents[i].z
			);
			vertex.BiTangent = bitangent;

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				indices.push_back(mesh->mFaces[i].mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0 && m_Materials.find(mesh->mMaterialIndex) == m_Materials.end())
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<std::shared_ptr<ITexture>> diffuseMaps = __loadTexturesFromMat(material, aiTextureType_DIFFUSE, ETexture::Diffuse);
			std::vector<std::shared_ptr<ITexture>> specularMaps = __loadTexturesFromMat(material, aiTextureType_SPECULAR, ETexture::Specular);
			std::vector<std::shared_ptr<ITexture>> normalMaps = __loadTexturesFromMat(material, aiTextureType_HEIGHT, ETexture::Normal);

			std::shared_ptr<IMaterial> pMat = IMaterial::Create();
			pMat->fetchTextureOfType(ETexture::Diffuse).insert(pMat->fetchTextureOfType(ETexture::Diffuse).end(), diffuseMaps.begin(), diffuseMaps.end());
			pMat->fetchTextureOfType(ETexture::Specular).insert(pMat->fetchTextureOfType(ETexture::Specular).end(), specularMaps.begin(), specularMaps.end());
			pMat->fetchTextureOfType(ETexture::Normal).insert(pMat->fetchTextureOfType(ETexture::Normal).end(), normalMaps.begin(), normalMaps.end());
			m_Materials[mesh->mMaterialIndex] = pMat;
		}

		m_Meshes.emplace_back(IMesh::Create(
			std::move(vertices),
			std::move(indices), 
			mesh->mMaterialIndex, 
			GL_TRIANGLES
		));
	}
	std::vector<std::shared_ptr<ITexture>> CModelLoader::__loadTexturesFromMat(aiMaterial* mat, int vAiType, ETexture type)
	{
		std::vector<std::shared_ptr<ITexture>> textures;
		aiTextureType aiType = static_cast<aiTextureType>(vAiType);
		for (unsigned int i = 0; i < mat->GetTextureCount(aiType); ++i)
		{
			aiString str;
			mat->GetTexture(aiType, i, &str);
			std::filesystem::path path = m_ModelPath / str.C_Str();

			if (m_TexCache.contains(path))
			{
				textures.push_back(m_TexCache[path]);
			}
			else
			{
				std::shared_ptr<ITexture> texture = ITexture::Create(path, type);
				textures.push_back(texture);
				m_TexCache[path] = texture;
			}
		}
		return textures;
	}

}