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

		Skybox,
		Equirectangular,
		IrradianceMap,
		PreFilterMap,
		BRDFLUT
	};

	class PALE_API ITexture
	{
	public:
		ETexture Type;
		virtual unsigned int getID() const = 0;
		static std::shared_ptr<ITexture> Create(const std::filesystem::path& vPath, ETexture vType, unsigned int vInternalFormat = 0);
		static std::shared_ptr<ITexture> Create(const std::vector<std::filesystem::path>& vFaces, ETexture vType);
		static std::shared_ptr<ITexture> CreateIrradianceMap(std::shared_ptr<ITexture>& vSkybox, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat = 0);
		static std::shared_ptr<ITexture> CreatePreFilterMap(std::shared_ptr<ITexture>& vSkybox, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat = 0);
		static std::shared_ptr<ITexture> CreateBRDFLUT(unsigned int vWidth, unsigned int vHeight);
	};
}