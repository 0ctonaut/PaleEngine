#pragma once
#include <PaleRenderer/PaleRendererOpenGL.h>
#include "Panels/HierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ViewportPanel.h"

namespace PaleUI
{
    class CUIManager
    {
    public:
        CUIManager();
        ~CUIManager();
        void OnUISetup(GLFWwindow* vWindow);

        void OnUIBeginFrame();
        void OnUIRender(PaleRdr::CScene& vScene, std::shared_ptr<PaleRdr::CFrameBufferOpenGL> vFrameBuffer);
        void OnUIEndFrame();
        void OnUIDestroy();

        void useDockSpace(bool* p_open);
    public:
        Event<unsigned int, unsigned int> EventViewportResize;

    private:
        CHierarchyPanel* m_HierarchyPanel;
        CInspectorPanel* m_InspectorPanel;
        CViewportPanel* m_ViewportPanel;
        
    };
}