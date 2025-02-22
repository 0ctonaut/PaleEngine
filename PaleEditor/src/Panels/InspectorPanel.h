#pragma once
#include <entt/entt.hpp>

namespace PaleUI
{
    class CInspectorPanel
    {
    public:
        CInspectorPanel();
        ~CInspectorPanel();

        void OnUIRender(entt::registry& vRegistry);
        void OnSingleSelection(const entt::entity& vId);

    private:
        void __renderUICompName(entt::registry& vRegistry, const entt::entity& vId);
        void __renderUICompTrans(entt::registry& vRegistry, const entt::entity& vId);
        void __renderUICompLight(entt::registry& vRegistry, const entt::entity& vId);
        void __renderUICompMeshRdr(entt::registry& vRegistry, const entt::entity& vId);
        void __renderUICompSkybox(entt::registry& vRegistry, const entt::entity& vId);

        // ---------------------------

        void __drawVec3Control(
            const std::string& vLabel,
            glm::vec3& vValues,
            const glm::vec3& vResetValue,
            float vMin,
            float vMax,
            float vSpeed = 0.1f,
            float vColumnWidth = 200.f);

        void __drawDisplayTextureButton(const std::string& label, unsigned int textureID,
            float vImageButtonSize = 16.0f, float vPreviewImageSize = 256.0f);

    private:
        entt::entity m_SelectionEntity;
    };
}