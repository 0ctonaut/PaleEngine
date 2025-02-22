#include "stdafx.h"
#include "PaleRenderer/Material/Texture.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"

namespace PaleRdr
{
	std::shared_ptr<ITexture> ITexture::Create(const std::filesystem::path& vPath, ETexture vType, unsigned int vInternalFormat)
	{
		if (vType == ETexture::Skybox) // create skybox from Equirectangular
		{
			return std::make_shared<CTextureSkyboxOpenGL>(vPath.string(), vInternalFormat);
		}
		else
		{
			return std::make_shared<CTexture2DOpenGL>(vPath.string(), vType, vInternalFormat);
		}
	}
	std::shared_ptr<ITexture> ITexture::Create(const std::vector<std::filesystem::path>& vFaces, ETexture vType)
	{
		PALE_RDR_ASSERT(vType == ETexture::Skybox, "Need skybox type!");
		return std::make_shared<CTextureSkyboxOpenGL>(vFaces);
	}

	std::shared_ptr<ITexture> ITexture::CreateIrradianceMap(std::shared_ptr<ITexture>& vSkybox, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat)
	{
		return std::make_shared<CTextureSkyboxOpenGL>(ETexture::IrradianceMap, vSkybox, vWidth, vHeight, vInternalFormat);
	}

	std::shared_ptr<ITexture> ITexture::CreatePreFilterMap(std::shared_ptr<ITexture>& vSkybox, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat)
	{
		return std::make_shared<CTextureSkyboxOpenGL>(ETexture::PreFilterMap, vSkybox, vWidth, vHeight, vInternalFormat);
	}

	std::shared_ptr<ITexture> ITexture::CreateBRDFLUT(unsigned int vWidth, unsigned int vHeight)
	{
		return std::make_shared<CTexture2DOpenGL>(ETexture::BRDFLUT, vWidth, vHeight, GL_RG16F);
	}
}