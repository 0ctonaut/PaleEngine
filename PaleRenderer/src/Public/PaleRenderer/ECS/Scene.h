#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"

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
        void OnRender(const glm::mat4& vModel);

    private:
        void __BeforeRenderMeshRdr();
        void __OnRenderMeshRdr(const glm::mat4& vModel);

    private:
        glm::vec4 m_BackgroundColor;

        // ECS
        entt::registry m_Registry;
        std::vector<entt::entity> m_Entities;
    };
}