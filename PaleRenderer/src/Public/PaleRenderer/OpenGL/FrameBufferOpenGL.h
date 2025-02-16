#pragma once
#include "PaleRenderer/Core/PaleRendererExport.h"
namespace PaleRdr
{
	class PALE_API CFrameBufferOpenGL
	{
	public:
		CFrameBufferOpenGL(unsigned int vWidth, unsigned int vHeight);
		~CFrameBufferOpenGL();
		CFrameBufferOpenGL(const CFrameBufferOpenGL&) = delete;

		void bind();
		void unbind();
		void OnResize(unsigned int vWidth, unsigned int vHeight);
		unsigned int getColorBufferID() const { return m_ColorBufferID;	}
		unsigned int getDepthBufferID() const { return m_DepthBufferID; }
		unsigned int getDepthDisplayBufferID() const { return m_DepthDisplayBufferID; }

	private:
		void __resetColorBuffer(unsigned int vWidth, unsigned int vHeight);
		void __resetDepthBuffer(unsigned int vWidth, unsigned int vHeight);

		unsigned int m_ID;
		unsigned int m_ColorBufferID;
		unsigned int m_DepthBufferID;
		unsigned int m_DepthDisplayBufferID;
	};
}