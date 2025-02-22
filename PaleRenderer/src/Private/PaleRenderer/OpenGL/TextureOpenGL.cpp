#include "stdafx.h"
#include "PaleRenderer/OpenGL/TextureOpenGL.h"
// ------
#define STB_IMAGE_IMPLEMENTATION
#include "PaleRenderer/Core/stb_image.h"
// ------
#include "PaleRenderer/Core/PathManager.h"
#include "PaleRenderer/Core/Log.h"
#include "PaleRenderer/Material/Shader.h"
#include "PaleRenderer/Mesh/Cube.h"
#include "PaleRenderer/Mesh/Quad.h"

glm::mat4 gCaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 gCaptureViews[] =
{
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};
unsigned int gCaptureFBO, gCaptureRBO;

namespace PaleRdr
{
    CTexture2DOpenGL::CTexture2DOpenGL(const std::filesystem::path& vPath, ETexture vType, unsigned int vInternalFormat)
    { 
        Type = vType;
        __initTex();
        __loadImage(vPath.string(), vInternalFormat);
    }

    CTexture2DOpenGL::CTexture2DOpenGL(ETexture vType, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat)
    {
        Type = vType;
        __initTex();
        glTexImage2D(GL_TEXTURE_2D, 0, vInternalFormat == GL_NONE ? GL_RG16F : vInternalFormat, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (vType == ETexture::BRDFLUT)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, gCaptureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, gCaptureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0);

            std::shared_ptr<IShader> pShaderCapture = IShader::Create(
                CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/brdf.vert",
                CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/brdf.frag");

            glViewport(0, 0, vWidth, vHeight);
            pShaderCapture->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto pQuadMesh = Quad::getMeshes()[0];
            glBindVertexArray(pQuadMesh->getVAO());
            glDrawElements(pQuadMesh->getElementType(), pQuadMesh->getIndiceSize(), GL_UNSIGNED_INT, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else
        {
            PALE_RDR_ERROR("Not supported type!");
        }
    }

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
    void CTexture2DOpenGL::__loadImage(const std::string& vPath, unsigned int vInternalFormat)
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
        GLenum format = vInternalFormat;

        bool success = false;
        if (vInternalFormat == GL_RGB16F || vInternalFormat == GL_RGB32F || vInternalFormat == GL_RGBA16F || vInternalFormat == GL_RGBA32F)
        {
            float* data = stbi_loadf(vPath.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_FLOAT, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                success = true;
            }
            stbi_image_free(data);
        }
        else
        {
            unsigned char* data = stbi_load(vPath.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                if (format == GL_NONE)
                {
                    if (nrChannels == 1)
                        format = GL_RED;
                    else if (nrChannels == 3)
                        format = GL_RGB;
                    else if (nrChannels == 4)
                        format = GL_RGBA;
                }
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                success = true;
            }
            stbi_image_free(data);
        }
        if (!success)
        {
            PALE_RDR_ERROR("Failed to load texture %s", vPath);
        }
    }

    // ----------------------------------------------------------------------------------------------

    CTextureSkyboxOpenGL::CTextureSkyboxOpenGL(const std::filesystem::path& vHDR, unsigned int vInternalFormat)
    {        
        Type = ETexture::Skybox;
        __initTex();        
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, vInternalFormat==GL_NONE? GL_RGB16F : vInternalFormat, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        std::shared_ptr<ITexture> pTextureEquirectangular = std::make_shared<CTexture2DOpenGL>(vHDR, ETexture::Equirectangular, GL_RGB32F);

        glGenFramebuffers(1, &gCaptureFBO);
        glGenRenderbuffers(1, &gCaptureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, gCaptureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, gCaptureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gCaptureRBO);

        std::shared_ptr<IShader> pShaderCapture = IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/capture.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/equirectangular2Skybox.frag");
        pShaderCapture->use();
        pShaderCapture->setUniform("equirectangularMap", static_cast<int>(0));
        pShaderCapture->setUniform("projection", gCaptureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pTextureEquirectangular->getID());

        glViewport(0, 0, 512, 512);
        glBindFramebuffer(GL_FRAMEBUFFER, gCaptureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            pShaderCapture->use();
            pShaderCapture->setUniform("view", gCaptureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TexID, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                PALE_RDR_ERROR("Framebuffer is not complete!");
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto pMesh = Cube::getMeshes()[0];
            glBindVertexArray(pMesh->getVAO());
            glDrawElements(pMesh->getElementType(), pMesh->getIndiceSize(), GL_UNSIGNED_INT, 0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    CTextureSkyboxOpenGL::CTextureSkyboxOpenGL(const std::vector<std::filesystem::path>& vFaces, unsigned int vInternalFormat)
    {
        Type = ETexture::Skybox;
        __initTex();
        __loadImage(vFaces);
    }

    CTextureSkyboxOpenGL::CTextureSkyboxOpenGL(ETexture vType, std::shared_ptr<ITexture>& vSkybox, unsigned int vWidth, unsigned int vHeight, unsigned int vInternalFormat)
    {
        Type = vType;
        __initTex();
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, vInternalFormat == GL_NONE ? GL_RGB16F : vInternalFormat, vWidth, vHeight, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, gCaptureFBO);
        auto pCubeMesh = Cube::getMeshes()[0];
        if (vType == ETexture::IrradianceMap)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, gCaptureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, vWidth, vHeight);

            glViewport(0, 0, vWidth, vHeight);
            std::shared_ptr<IShader> pShaderCapture = IShader::Create(
                CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/capture.vert",
                CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/irradiance.frag");
            pShaderCapture->use();
            pShaderCapture->setUniform("skybox", static_cast<int>(0));
            pShaderCapture->setUniform("projection", gCaptureProjection);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, vSkybox->getID());
            for (unsigned int i = 0; i < 6; ++i)
            {
                pShaderCapture->use();
                pShaderCapture->setUniform("view", gCaptureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TexID, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glBindVertexArray(pCubeMesh->getVAO());
                glDrawElements(pCubeMesh->getElementType(), pCubeMesh->getIndiceSize(), GL_UNSIGNED_INT, 0);
            }
        }
        else if (vType == ETexture::PreFilterMap)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // IMPORTANT!!
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            std::shared_ptr<IShader> pShaderCapture = IShader::Create(
                CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/capture.vert",
                CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/prefilter.frag");
            pShaderCapture->use();
            pShaderCapture->setUniform("skybox", static_cast<int>(0));
            pShaderCapture->setUniform("projection", gCaptureProjection);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, vSkybox->getID());
            unsigned int maxMipLevels = 5;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                // reisze framebuffer according to mip-level size.
                unsigned int mipWidth = static_cast<unsigned int>(vWidth * std::pow(0.5, mip));
                unsigned int mipHeight = static_cast<unsigned int>(vHeight * std::pow(0.5, mip));
                glBindRenderbuffer(GL_RENDERBUFFER, gCaptureRBO);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float)mip / (float)(maxMipLevels - 1);
                pShaderCapture->setUniform("roughness", roughness);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    pShaderCapture->setUniform("view", gCaptureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TexID, mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glBindVertexArray(pCubeMesh->getVAO());
                    glDrawElements(pCubeMesh->getElementType(), pCubeMesh->getIndiceSize(), GL_UNSIGNED_INT, 0);
                }
            }
        }
        else
        {
            PALE_RDR_ERROR("Not supported type!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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