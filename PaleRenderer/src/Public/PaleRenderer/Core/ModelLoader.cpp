#include "stdafx.h"
#include "PaleRenderer/Core/ModelLoader.h"

namespace PaleRdr
{
	std::vector<CMeshOpenGL> IModelLoader::getCubeMeshes()
	{
		std::vector<CMeshOpenGL> meshes;

		std::vector<CVertexOpenGL> vertices = {
			{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},

			{{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},

			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},

			{{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},

			{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},

			{{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}
		};

		std::vector<unsigned int> indices = {
			0,	1,	2,	3,	4,	5,
			6,	7,	8,	9,	10, 11,
			12, 13, 14, 15, 16, 17,
			18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35
		};
		meshes.push_back(CMeshOpenGL(vertices, indices, false, false));
		return meshes;
	}

	// --------------------------------------------------------------------------------------

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

	CMeshOpenGL CModelLoader::__processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<CVertexOpenGL> vertices;
		std::vector<CTextureOpenGL> textures;
		std::vector<unsigned int> indices;

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			CVertexOpenGL vertex;
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

		unsigned int numTex = 0;
		unsigned int numNormalMap = 0;

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<CTextureOpenGL> diffuseMaps = __loadTexturesFromMat(material, aiTextureType_DIFFUSE, ETexture::Diffuse);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<CTextureOpenGL> specularMaps = __loadTexturesFromMat(material, aiTextureType_SPECULAR, ETexture::Specular);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			std::vector<CTextureOpenGL> normalMaps = __loadTexturesFromMat(material, aiTextureType_HEIGHT, ETexture::Normal);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}

		return CMeshOpenGL(
			std::move(vertices), 
			std::move(textures), 
			std::move(indices),
			true, true
			);
	}
	std::vector<CTextureOpenGL> CModelLoader::__loadTexturesFromMat(aiMaterial* mat, aiTextureType aiType, ETexture type, bool vFlip)
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
				CTextureOpenGL texture(path, vFlip);
				texture.Type = type;
				textures.push_back(texture);
				m_TexCache[path] = texture;
			}
		}
		return textures;
	}

}