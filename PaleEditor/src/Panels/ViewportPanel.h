#pragma once
#include <PaleRenderer/Common/Event.h>

namespace PaleUI
{
    class CViewportPanel
    {
    public:
        CViewportPanel(Event<unsigned int, unsigned int>* vEventViewportResize);
        ~CViewportPanel();

        void OnUIRender(std::shared_ptr<PaleRdr::CFrameBufferOpenGL> vFrameBuffer);

    public:
    
    private:
        ImVec2 m_PrevViewportSize;
        Event<unsigned int, unsigned int>* m_EventViewportResize;
    };
}