#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/Core/Camera.h"

namespace PaleRdr
{
    class CPassOpenGL;
    class PALE_API CScene
    {
    public:
        CScene();
        ~CScene();
        entt::entity addEntity(const std::string& vName = "Empty-Object");
        inline entt::registry& fetchRegistry() { return m_Registry; }
        void OnRender();

        inline PaleRdr::CCamera* fetchSceneCam() { return m_pCamera; }

    private:
        void __BeforeRenderMeshRdr();
        void __OnRenderMeshRdr();

    private:
        // Scene
        glm::vec4 m_BackgroundColor;
        PaleRdr::CCamera* m_pCamera;

        // ECS
        entt::registry m_Registry;
        std::vector<entt::entity> m_Entities;
    };
}