#include "stdafx.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"
// ------
#define STB_IMAGE_IMPLEMENTATION
#include "PaleRenderer/Core/stb_image.h"
// ------
#include "PaleRenderer/Core/PathManager.h"
#include "PaleRenderer/Core/Log.h"

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
            PALE_RDR_ERROR("{} doesn't exist.", vPath);
            return;
        }
        else
        {
            PALE_RDR_DEBUG("load texture from {}.", vPath);
        }

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(vPath.c_str(), &width, &height, &nrChannels, 0);
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            PALE_RDR_ERROR("Failed to load texture");
        }
        stbi_image_free(data);
    }
}