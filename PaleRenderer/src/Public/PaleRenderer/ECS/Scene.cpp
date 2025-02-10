#include "stdafx.h"
#include "Scene.h"
#include "PaleRenderer/ECS/NameComp.h"
#include "PaleRenderer/ECS/TransformComp.h"
#include "PaleRenderer/ECS/MeshRendererComp.h"
#include "PaleRenderer/OpenGL/PassOpenGL.h"

namespace PaleRdr
{
    CScene::CScene()
    {
        glEnable(GL_DEPTH_TEST);
        m_BackgroundColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    }

    CScene::~CScene()
    {
    }

    entt::entity CScene::addEntity(const std::string& vName)
    {
        entt::entity entity = m_Registry.create();
        m_Registry.emplace<PaleRdr::SCompName>(entity, vName);
        m_Registry.emplace<PaleRdr::SCompTransform>(entity);

        m_Entities.push_back(entity);
        return entity;
    }

    void CScene::OnRender(const glm::mat4& vModel)
    {
        __BeforeRenderMeshRdr();
        __OnRenderMeshRdr(vModel);
    }

    void CScene::__BeforeRenderMeshRdr()
    {
        glClearColor(
            m_BackgroundColor.x * m_BackgroundColor.w, 
            m_BackgroundColor.y * m_BackgroundColor.w, 
            m_BackgroundColor.z * m_BackgroundColor.w, 
            m_BackgroundColor.w
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void CScene::__OnRenderMeshRdr(const glm::mat4& vModel)
    {
        for (auto& id : m_Entities)
        {
            auto* meshrdr = m_Registry.try_get<PaleRdr::SCompMeshRenderer>(id);
            auto* trans = m_Registry.try_get<PaleRdr::SCompTransform>(id);

            meshrdr->_Pass.setMat4("model", vModel * trans->getTransfrom());
            for (auto& mesh : meshrdr->_Meshes)
            {
                mesh.draw(meshrdr->_Pass);
            }
            meshrdr->_Pass.setMat4("model", vModel);
        }
    }
}