#include "stdafx.h"
#include "PaleRenderer/OpenGL/WindowOpenGL.h"

namespace PaleRdr
{
	CWindowOpenGL::CWindowOpenGL(const std::string& vName, unsigned int vWidth, unsigned int vHeight)
	{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_pWindow = glfwCreateWindow(vWidth, vHeight, vName.c_str(), NULL, NULL);
        if (m_pWindow == NULL)
        {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
        }
        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            spdlog::error("Failed to initialize GLAD");
        }
	}
    CWindowOpenGL::~CWindowOpenGL()
    {
        glfwDestroyWindow(m_pWindow);
    }

    void CWindowOpenGL::OnResize(unsigned int vWidth, unsigned int vHeight) const
    {
        glViewport(0, 0, vWidth, vHeight);
    }
    void CWindowOpenGL::renameWindow(std::string vName) const
    {
        glfwSetWindowTitle(m_pWindow, vName.c_str());
    }
}