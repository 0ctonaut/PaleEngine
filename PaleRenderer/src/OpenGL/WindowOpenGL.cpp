#pragma once
#include "WindowOpenGL.h"
#include "stdafx.h"

namespace PaleRenderer
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
}