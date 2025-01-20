#pragma once
#include <string>
class CTextureOpenGL
{
public:
	CTextureOpenGL(const std::string& vPath);

	inline unsigned int getTexID() const { return m_TexID;	}

private:
	unsigned int m_TexID;
};
