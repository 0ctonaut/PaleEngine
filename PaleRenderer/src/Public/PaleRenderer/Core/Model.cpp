#include "stdafx.h"
#include "PaleRenderer/Core/Model.h"

namespace PaleRdr
{
	CModel::CModel(std::filesystem::path vPath)
	{
		load(vPath);
	}
	bool CModel::load(std::filesystem::path vPath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			vPath.string(), 
			aiProcess_Triangulate | aiProcess_FlipUVs
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

	std::vector<CMeshOpenGL> CModel::getCubeMeshes()
	{
		std::vector<CMeshOpenGL> meshes;

		std::vector<CVertexOpenGL> vertices = {
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // 0
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // 1
			{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // 2
			{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // 3

			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}, // 4
			{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}, // 5
			{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}}, // 6
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}, // 7
		};

		std::vector<unsigned int> indices = {
			// F
			0, 1, 2,
			2, 3, 0,
			// B
			4, 5, 6,
			6, 7, 4,
			// L
			4, 0, 3,
			3, 7, 4,
			// R
			1, 5, 6,
			6, 2, 1,
			// T
			3, 2, 6,
			6, 7, 3,
			// B
			4, 5, 1,
			1, 0, 4
		};
		meshes.push_back(CMeshOpenGL(vertices, {}, indices));
		return meshes;
	}

	bool CModel::__processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(__processMesh(mesh, scene));
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

	CMeshOpenGL CModel::__processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<CVertexOpenGL> vertices;
		std::vector<CTextureOpenGL> textures;
		std::vector<unsigned int> indices;
		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			glm::vec3 position(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

			glm::vec3 normal(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);

			glm::vec2 texcoord(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);

			CVertexOpenGL vertex;
			vertex.Position = position;
			vertex.Normal = normal;
			vertex.TexCoord = texcoord;
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				indices.push_back(mesh->mFaces[i].mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<CTextureOpenGL> diffuseMaps = __loadTexturesFromMat(material, aiTextureType_DIFFUSE, ETexture::Diffuse);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<CTextureOpenGL> specularMaps = __loadTexturesFromMat(material, aiTextureType_SPECULAR, ETexture::Specular);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return CMeshOpenGL(std::move(vertices), std::move(textures), std::move(indices));
	}
	std::vector<CTextureOpenGL> CModel::__loadTexturesFromMat(aiMaterial* mat, aiTextureType aiType, ETexture type)
	{
		std::vector<CTextureOpenGL> textures;
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
				CTextureOpenGL texture(path);
				texture.Type = type;
				textures.push_back(texture);
				m_TexCache[path] = texture;
			}
		}
		return textures;
	}
}