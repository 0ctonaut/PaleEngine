#include "stdafx.h"
#include "PaleRenderer/Core/Camera.h"
#include "PaleRenderer/Core/Application.h"

namespace PaleRdr
{
	void CCamera::OnRender()
	{

	}
	void CCamera::OnViewportResize(unsigned int vWidth, unsigned int vHeight)
	{
		m_ViewportWidth = vWidth;
		m_ViewportHeight = vHeight;
	}
}