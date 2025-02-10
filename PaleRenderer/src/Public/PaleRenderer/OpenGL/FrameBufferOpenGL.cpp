#include "stdafx.h"
#include "PaleRenderer/OpenGL/FrameBufferOpenGL.h"

namespace PaleRdr
{
	CFrameBufferOpenGL::CFrameBufferOpenGL(unsigned int vWidth, unsigned int vHeight)
	{
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        glGenTextures(1, &m_ColorBufferID);
        __resetColorBuffer(vWidth, vHeight);

        glGenTextures(1, &m_DepthBufferID);
        __resetDepthBuffer(vWidth, vHeight);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	CFrameBufferOpenGL::~CFrameBufferOpenGL()
	{

	}

    void CFrameBufferOpenGL::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    }

    void CFrameBufferOpenGL::OnResize(unsigned int vWidth, unsigned int vHeight)
    {
        __resetColorBuffer(vWidth, vHeight);
        __resetDepthBuffer(vWidth, vHeight);
    }
    void CFrameBufferOpenGL::__resetColorBuffer(unsigned int vWidth, unsigned int vHeight)
    {
        glBindTexture(GL_TEXTURE_2D, m_ColorBufferID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vWidth, vHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferID, 0);
    }
    void CFrameBufferOpenGL::__resetDepthBuffer(unsigned int vWidth, unsigned int vHeight)
    {
        glBindTexture(GL_TEXTURE_2D, m_DepthBufferID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, vWidth, vHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferID, 0);
    }
}