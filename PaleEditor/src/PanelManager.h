#pragma once
#include <PaleRenderer/PaleRendererOpenGL.h>
#include "Panels/HierarchyPanel.h"
#include "Panels/InspectorPanel.h"

namespace PaleUI
{
    class CPanelManager
    {
    public:
        CPanelManager();
        ~CPanelManager();

        void OnUIRender(PaleRdr::CScene& vScene);
    private:
        CHierarchyPanel* m_HierarchyPanel;
        CInspectorPanel* m_InspectorPanel;
    };
}