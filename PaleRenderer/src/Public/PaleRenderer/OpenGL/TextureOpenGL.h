#pragma once
#include <string>
#include <filesystem>
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
	enum class ETexture
	{
		Diffuse,
		Specular,
		Normal
	};

	class PALE_API CTextureOpenGL
	{
	public:
		CTextureOpenGL();
		CTextureOpenGL(const std::string& vPath, bool vFlip = true);
		CTextureOpenGL(const std::filesystem::path& vPath, bool vFlip = true);
		// CTextureOpenGL(const CTextureOpenGL&) = delete;
		~CTextureOpenGL();

		inline unsigned int getTexID() const { return m_TexID; }
		ETexture Type;

	private:
		void __initTex();
		void __loadImage(const std::string& vPath, bool vFlip = true);
		unsigned int m_TexID;
	};
}
