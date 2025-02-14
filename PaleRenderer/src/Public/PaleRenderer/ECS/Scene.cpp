#include "stdafx.h"
#include "Scene.h"
#include "PaleRenderer/ECS/NameComp.h"
#include "PaleRenderer/ECS/TransformComp.h"
#include "PaleRenderer/ECS/MeshRendererComp.h"
#include "PaleRenderer/ECS/LightComp.h"
#include "PaleRenderer/OpenGL/PassOpenGL.h"
#include "PaleRenderer/Core/Application.h"

namespace PaleRdr
{
    CScene::CScene()
    {
        glEnable(GL_DEPTH_TEST);
        m_BackgroundColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
        m_pSceneCamera = new CCamera(glm::vec3(0.0f, 0.0f, 10.0f));
    }

    CScene::~CScene()
    {
        delete m_pSceneCamera;
    }

    entt::entity CScene::addEntity(
        const std::string& vName, 
        const glm::vec3& vPosition, 
        const glm::vec3& vRotation, 
        const glm::vec3& vScale
    )
    {
        entt::entity entity = m_Registry.create();
        m_Registry.emplace<PaleRdr::SCompName>(entity, vName);
        m_Registry.emplace<PaleRdr::SCompTransform>(entity, vPosition, vRotation, vScale);

        m_Entities.push_back(entity);
        return entity;
    }

    void CScene::OnRender(std::shared_ptr<CFrameBufferOpenGL> vFrameBuffer)
    {
        vFrameBuffer->bind();
        __BeforeRender();
        __OnRenderLight();
        __OnRenderMeshRdr();
        vFrameBuffer->unbind();
    }

    void CScene::__BeforeRender()
    {
        glClearColor(
            m_BackgroundColor.x * m_BackgroundColor.w, 
            m_BackgroundColor.y * m_BackgroundColor.w, 
            m_BackgroundColor.z * m_BackgroundColor.w, 
            m_BackgroundColor.w
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void CScene::__OnRenderLight()
    {
        for (auto& id : m_Entities)
        {
            if (auto* light = m_Registry.try_get<PaleRdr::SCompPointLight>(id))
            {
                auto* trans = m_Registry.try_get<PaleRdr::SCompTransform>(id);
                m_SceneLights.push_back({trans->_Position, light->_Color, light->_Intensity});
            }
        }
    }

    void CScene::__OnRenderMeshRdr()
    {
        for (auto& id : m_Entities)
        {
            auto* meshrdr = m_Registry.try_get<PaleRdr::SCompMeshRenderer>(id);
            auto* trans = m_Registry.try_get<PaleRdr::SCompTransform>(id);

            meshrdr->_Pass.use();

            glm::mat4 uProject = m_pSceneCamera->getProjectionMatrix();
            glm::mat4 uView = m_pSceneCamera->getViewMatrix();
            glm::mat4 uModel = trans->getTransfrom();

            meshrdr->_Pass.setMat4("uProjection", uProject);
            meshrdr->_Pass.setMat4("uView", uView);
            meshrdr->_Pass.setMat4("uModel", uModel);
            meshrdr->_Pass.setMat3("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(uModel))));


            if (meshrdr->_bLit)
            {
                for (int i = 0; i < m_SceneLights.size(); ++i)
                {
                    meshrdr->_Pass.setVec3("uLightPos", m_SceneLights[i]._Position);
                    meshrdr->_Pass.setVec3("uLightColor", m_SceneLights[i]._Color);
                    meshrdr->_Pass.setFloat("uLightIntensity", m_SceneLights[i]._Intensity);
                }
                meshrdr->_Pass.setVec3("uViewPos", m_pSceneCamera->Position);
            }

            if (auto* light = m_Registry.try_get<PaleRdr::SCompPointLight>(id))
            {
                meshrdr->_Pass.setVec3("uLightColor", light->_Color);
                meshrdr->_Pass.setFloat("uIntensity", light->_Intensity);
            }

            for (auto& mesh : meshrdr->_Meshes)
            {
                mesh.draw(meshrdr->_Pass);
            }
        }
    }
}