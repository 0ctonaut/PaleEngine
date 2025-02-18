#include "stdafx.h"
#include "PaleRenderer/Material/Texture.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"

namespace PaleRdr
{
	std::shared_ptr<ITexture> ITexture::Create(const std::string& vPath, ETexture vType)
	{
		return std::make_shared<CTexture2DOpenGL>(vPath, vType);
	}
	std::shared_ptr<ITexture> ITexture::Create(const std::filesystem::path& vPath, ETexture vType)
	{
		return std::make_shared<CTexture2DOpenGL>(vPath.string(), vType);
	}
	std::shared_ptr<ITexture> ITexture::Create(const std::vector<std::filesystem::path>& vFaces, ETexture vType)
	{
		PALE_RDR_ASSERT(vType == ETexture::Skybox, "Need skybox type!");
		return std::make_shared<CTextureSkyboxOpenGL>(vFaces);
	}
}