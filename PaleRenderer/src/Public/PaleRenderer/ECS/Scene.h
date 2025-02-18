#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/Core/Camera.h"

namespace PaleRdr
{
    class CFrameBufferOpenGL;

    struct SSceneLight
    {
        glm::vec3 _Position;
        glm::vec3 _Color;
        float _Intensity;
    };

    class PALE_API CScene
    {
    public:
        CScene();
        ~CScene();
        entt::entity addEntity(
            const std::string& vName = "Empty-Object",
            const glm::vec3& vPosition = glm::vec3(0.0),
            const glm::vec3& vRotation = glm::vec3(0.0),
            const glm::vec3& vScale = glm::vec3(1.0)
        );
        inline entt::registry& fetchRegistry() { return m_Registry; }
        void OnRender(std::shared_ptr<CFrameBufferOpenGL> vFrameBuffer);

        inline PaleRdr::CCamera* fetchSceneCam() { return m_pSceneCamera; }

    private:
        void __BeforeRender();
        void __OnRenderSkybox();
        void __OnRenderLight();
        void __OnRenderMeshRdr();
        void __AfterRender();

    private:
        // Scene
        glm::vec4 m_BackgroundColor;
        PaleRdr::CCamera* m_pSceneCamera;

        // --- need cleaning up every frame ---
        std::vector<SSceneLight> m_SceneLights;
        // --- ---

        // ECS
        entt::registry m_Registry;
        std::vector<entt::entity> m_Entities;
    };
}