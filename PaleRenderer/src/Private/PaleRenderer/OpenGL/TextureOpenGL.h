#pragma once
#include <string>
#include <filesystem>
#include "PaleRenderer/Material/Texture.h"

namespace PaleRdr
{
	class CTextureOpenGL : public ITexture
	{
	public:
		CTextureOpenGL();
		CTextureOpenGL(const std::string& vPath, ETexture vType);
		CTextureOpenGL(const std::filesystem::path& vPath, ETexture vType);

		~CTextureOpenGL();
		virtual inline unsigned int getID() const override { return m_TexID; }
		

	private:
		void __initTex();
		void __loadImage(const std::string& vPath);
		unsigned int m_TexID;
	};
}
