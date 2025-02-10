#pragma once
#include <string>
#include <filesystem>
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
	enum class ETexture
	{
		Diffuse,
		Specular
	};

	class PALE_API CTextureOpenGL
	{
	public:
		CTextureOpenGL();
		CTextureOpenGL(const std::string& vPath);
		CTextureOpenGL(const std::filesystem::path& vPath);

		inline unsigned int getTexID() const { return m_TexID; }
		ETexture Type;

	private:
		void __initTex();
		void __loadImage(const std::string& vPath);
		unsigned int m_TexID;
	};
}
