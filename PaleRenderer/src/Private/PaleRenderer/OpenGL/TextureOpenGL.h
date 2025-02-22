#pragma once
#include <string>
#include <filesystem>
#include "PaleRenderer/Material/Texture.h"

namespace PaleRdr
{
	class ITextureOpenGL : public ITexture
	{
	public:
		virtual inline unsigned int getID() const override { return m_TexID; }

	protected:
		unsigned int m_TexID;
	};

	class CTexture2DOpenGL : public ITextureOpenGL
	{
	public:
		CTexture2DOpenGL() = delete;
		CTexture2DOpenGL(const std::filesystem::path& vPath, ETexture vType, unsigned int vInternalFormat = 0);
		CTexture2DOpenGL(ETexture vType, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat = 0);

		~CTexture2DOpenGL();	

	private:
		void __initTex();
		void __loadImage(const std::string& vPath, unsigned int vInternalFormat);
	};

	class CTextureSkyboxOpenGL : public ITextureOpenGL
	{
	public:
		CTextureSkyboxOpenGL(const std::filesystem::path& vHDR, unsigned int vInternalFormat = 0);
		CTextureSkyboxOpenGL(const std::vector<std::filesystem::path>& vFaces, unsigned int vInternalFormat = 0);
		CTextureSkyboxOpenGL(ETexture vType, std::shared_ptr<ITexture>& vSkybox, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat = 0);

	private:
		void __initTex();
		void __loadImage(const std::vector<std::filesystem::path>& vFaces);
	};
}
