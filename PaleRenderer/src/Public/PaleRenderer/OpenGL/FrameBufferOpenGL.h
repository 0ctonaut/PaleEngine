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
		void OnResize(unsigned int vWidth, unsigned int vHeight);

		unsigned int m_ColorBufferID;
		unsigned int m_DepthBufferID;
	private:
		void __resetColorBuffer(unsigned int vWidth, unsigned int vHeight);
		void __resetDepthBuffer(unsigned int vWidth, unsigned int vHeight);

		unsigned int m_ID;
	};
}