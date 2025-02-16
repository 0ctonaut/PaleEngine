#include "stdafx.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"
// ------
#define STB_IMAGE_IMPLEMENTATION
#include "PaleRenderer/Core/stb_image.h"
// ------
#include "PaleRenderer/Core/PathManager.h"

namespace PaleRdr
{
    CTextureOpenGL::CTextureOpenGL()
    {
        __initTex();
    }

    CTextureOpenGL::CTextureOpenGL(const std::string& vPath, ETexture vType) : CTextureOpenGL()
    {
        Type = vType;
        __loadImage(vPath);
    }

    CTextureOpenGL::CTextureOpenGL(const std::filesystem::path& vPath, ETexture vType) :
        CTextureOpenGL(vPath.string(), vType) { }

    CTextureOpenGL::~CTextureOpenGL()
    {
        glDeleteTextures(1, &m_TexID);
    }
    void CTextureOpenGL::__initTex()
    {
        glGenTextures(1, &m_TexID);
        glBindTexture(GL_TEXTURE_2D, m_TexID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    void CTextureOpenGL::__loadImage(const std::string& vPath)
    {
        if (!std::filesystem::exists(vPath))
        {
            spdlog::error("{} doesn't exist.", vPath);
            return;
        }
        else
        {
            spdlog::trace("load texture from {}.", vPath);
        }

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(vPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            spdlog::error("Failed to load texture");
        }
        stbi_image_free(data);
    }
}