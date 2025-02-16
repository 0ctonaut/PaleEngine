#include "stdafx.h"
#include "PaleRenderer/Material/Texture.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"

namespace PaleRdr
{
	std::shared_ptr<ITexture> ITexture::Create(const std::string& vPath, ETexture vType)
	{
		return std::make_shared<CTextureOpenGL>(vPath, vType);
	}
	std::shared_ptr<ITexture> ITexture::Create(const std::filesystem::path& vPath, ETexture vType)
	{
		return std::make_shared<CTextureOpenGL>(vPath.string(), vType);
	}
}