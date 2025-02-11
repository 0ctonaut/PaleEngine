#include "stdafx.h"
#include "PaleRenderer/Core/Application.h"

namespace PaleRdr
{
	CApplication::CApplication() : m_Title("PaleEngine v0.1"), m_ViewportWidth(1920), m_ViewportHeight(1080)
	{
		m_pWindow = std::make_shared<CWindowOpenGL>(m_Title.c_str());
		m_pFrameBuffer = std::make_shared<CFrameBufferOpenGL>(m_ViewportWidth, m_ViewportHeight);
	}
	CApplication::~CApplication()
	{
	}
	void CApplication::OnViewportResize(unsigned int vWidth, unsigned int vHeight)
	{
		m_pFrameBuffer->OnResize(vWidth, vHeight);
		glViewport(0, 0, vWidth, vHeight);
		m_ViewportWidth = vWidth;
		m_ViewportHeight = vHeight;
	}
	void CApplication::OnRender(float vFrameRate)
	{
		m_pWindow->renameWindow(std::format("{} {:.2f} ms/frame ({:.2f} FPS)", m_Title, 1000.0f / vFrameRate, vFrameRate));
	}
}