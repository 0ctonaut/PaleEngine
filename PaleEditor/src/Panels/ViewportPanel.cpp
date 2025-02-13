#include "stdafx.h"
#include "ViewportPanel.h"
#include <PaleRenderer/Core/Application.h>

namespace PaleUI
{
    CViewportPanel::CViewportPanel(Event<unsigned int, unsigned int>* vEventViewportResize) : 
        m_EventViewportResize(vEventViewportResize)
    {
        m_PrevViewportSize = ImVec2(1920, 1080);
    }

    CViewportPanel::~CViewportPanel()
	{

	}

	void CViewportPanel::OnUIRender(std::shared_ptr<PaleRdr::CFrameBufferOpenGL> vFrameBuffer)
	{
        ImVec2 ViewportSize(0, 0);
        ImGui::Begin("Scene");
        ViewportSize = ImGui::GetContentRegionAvail();

        if (m_PrevViewportSize.x != ViewportSize.x || m_PrevViewportSize.y != ViewportSize.y)
        {
            m_EventViewportResize->invoke(ViewportSize.x, ViewportSize.y);
            m_PrevViewportSize = ViewportSize;
        }

        unsigned int ColorID = vFrameBuffer->getColorBufferID();
        ImGui::Image(ColorID, ViewportSize, { 0, 1 }, { 1, 0 });
        ImGui::End();
        
        ImGui::Begin("Depth");
        unsigned int DepthID = vFrameBuffer->getDepthBufferID();
        ImGui::Image(DepthID, ViewportSize, {0, 1}, {1, 0});
        ImGui::End();  
	}
}