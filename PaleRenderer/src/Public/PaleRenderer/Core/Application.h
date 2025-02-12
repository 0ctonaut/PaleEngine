#pragma once
#include "PaleRenderer/Common/Singleton.h"
#include "PaleRenderer/OpenGL/FrameBufferOpenGL.h"
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/OpenGL/WindowOpenGL.h"

namespace PaleRdr
{
	class PALE_API CApplication
	{
	public:
		CApplication();
		~CApplication();
		void OnViewportResize(unsigned int, unsigned int);
		void OnRender(float vFrameRate);
		const std::shared_ptr<CFrameBufferOpenGL> getFrameBuffer() const { return m_pFrameBuffer;	}
		inline unsigned int getViewportWidth() const { return m_ViewportWidth; }
		inline unsigned int getViewportHeight() const { return m_ViewportHeight; }
		inline float getDeltaTime() const { return m_DeltaTime; }
		std::shared_ptr<PaleRdr::CWindowOpenGL> fetchWindow() { return m_pWindow; }

	private:
		std::string m_Title;
		unsigned int m_ViewportWidth;
		unsigned int m_ViewportHeight;
		std::shared_ptr<CFrameBufferOpenGL> m_pFrameBuffer;
		std::shared_ptr<PaleRdr::CWindowOpenGL> m_pWindow;

		float m_DeltaTime, m_LastTime;
	};
}