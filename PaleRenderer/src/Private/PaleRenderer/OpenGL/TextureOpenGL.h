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
		CTexture2DOpenGL(const std::string& vPath, ETexture vType);
		CTexture2DOpenGL(const std::filesystem::path& vPath, ETexture vType);

		~CTexture2DOpenGL();	

	private:
		void __initTex();
		void __loadImage(const std::string& vPath);
	};

	class CTextureSkyboxOpenGL : public ITextureOpenGL
	{
	public:
		CTextureSkyboxOpenGL() = delete;
		CTextureSkyboxOpenGL(const std::vector<std::filesystem::path>& vFaces);

	private:
		void __initTex();
		void __loadImage(const std::vector<std::filesystem::path>& vFaces);
	};
}
