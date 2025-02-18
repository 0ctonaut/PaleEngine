#pragma once
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
	enum class PALE_API ETexture
	{
		Diffuse,
		Specular,
		Albedo,
		Normal,
		Metallic,
		Roughness,
		AO,

		Skybox
	};

	//enum class PALE_API ESkyboxFace
	//{
	//	POSITIVE_X,
	//	NEGATIVE_X,
	//	POSITIVE_Y,
	//	NEGATIVE_Y,
	//	POSITIVE_Z,
	//	NEGATIVE_Z
	//};

	class PALE_API ITexture
	{
	public:
		ETexture Type;
		virtual unsigned int getID() const = 0;
		static std::shared_ptr<ITexture> Create(const std::string& vPath, ETexture vType);
		static std::shared_ptr<ITexture> Create(const std::filesystem::path& vPath, ETexture vType);
		static std::shared_ptr<ITexture> Create(const std::vector<std::filesystem::path>& vFaces, ETexture vType);
	};
}