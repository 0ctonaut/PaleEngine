#pragma once
#include <string>
#include "Core/PaleRendererExport.h"
namespace PaleRenderer
{
	class PALE_API CTextureOpenGL
	{
	public:
		CTextureOpenGL(const std::string& vPath);

		inline unsigned int getTexID() const { return m_TexID; }

	private:
		unsigned int m_TexID;
	};
}
