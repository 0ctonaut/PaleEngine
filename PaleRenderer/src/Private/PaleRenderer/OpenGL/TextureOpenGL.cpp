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
    CTexture2DOpenGL::CTexture2DOpenGL(const std::string& vPath, ETexture vType)
    {
        Type = vType;
        __initTex();
        __loadImage(vPath);
    }

    CTexture2DOpenGL::CTexture2DOpenGL(const std::filesystem::path& vPath, ETexture vType) :
        CTexture2DOpenGL(vPath.string(), vType) { }

    CTexture2DOpenGL::~CTexture2DOpenGL()
    {
        glDeleteTextures(1, &m_TexID);
    }
    void CTexture2DOpenGL::__initTex()
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
    void CTexture2DOpenGL::__loadImage(const std::string& vPath)
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
        GLenum format = GL_RGB;
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
            PALE_RDR_ERROR("Failed to load texture %s", vPath);
        }
        stbi_image_free(data);
    }

    // ----------------------------------------------------------------------------------------------

    CTextureSkyboxOpenGL::CTextureSkyboxOpenGL(const std::vector<std::filesystem::path>& vFaces)
    {
        Type = ETexture::Skybox;
        __initTex();
        __loadImage(vFaces);
    }

    void CTextureSkyboxOpenGL::__initTex()
    {
        glGenTextures(1, &m_TexID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void CTextureSkyboxOpenGL::__loadImage(const std::vector<std::filesystem::path>& vFaces)
    {
        int width, height, nrChannels;

        for (int i = 0; i < 6; ++i)
        {
            if (!std::filesystem::exists(vFaces[i]))
            {
                PALE_RDR_ERROR("{} doesn't exist.", vFaces[i].string());
                return;
            }
            else
            {
                PALE_RDR_DEBUG("load texture from {}.", vFaces[i].string());
            }

            // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            unsigned char* data = stbi_load(vFaces[i].string().c_str(), &width, &height, &nrChannels, 0);
            GLenum format = GL_RGB;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                PALE_RDR_ERROR("Failed to load texture %s", vFaces[i].string().c_str());
            }
            stbi_image_free(data);
        }
    }
}