#include "stdafx.h"
#include "Scene.h"
#include "PaleRenderer/ECS/NameComp.h"
#include "PaleRenderer/ECS/TransformComp.h"
#include "PaleRenderer/ECS/MeshRendererComp.h"
#include "PaleRenderer/ECS/LightComp.h"
#include "PaleRenderer/Core/Application.h"
#include "PaleRenderer/Mesh/Mesh.h"
#include "PaleRenderer/Material/Material.h"
#include "PaleRenderer/OpenGL/FrameBufferOpenGL.h"

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
        __AfterRender();
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

            for (int i = 0; i < meshrdr->_Meshes.size(); ++i)
            {
                std::shared_ptr<IMesh>& Mesh = meshrdr->_Meshes[i];
                int MaterialIndex = Mesh->getMaterialIndex();
                std::shared_ptr<IMaterial>& pMaterial = meshrdr->_Materials[MaterialIndex];
                std::shared_ptr<IShader>& pShader = pMaterial->fetchShader();

                pShader->use();

                glm::mat4 uProject = m_pSceneCamera->getProjectionMatrix();
                glm::mat4 uView = m_pSceneCamera->getViewMatrix();
                glm::mat4 uModel = trans->getTransfrom();

                pShader->setUniform("uProjection", uProject);
                pShader->setUniform("uView", uView);
                pShader->setUniform("uModel", uModel);
                pShader->setUniform("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(uModel))));

                int numTex = 0;

                auto& diffuseTexture = pMaterial->fetchTextureOfType(ETexture::Diffuse);
                for (int i = 0; i < diffuseTexture.size(); ++i)
                {
                    glActiveTexture(GL_TEXTURE0 + numTex);
                    std::string name = std::format("tex_diffuse{}", i+1);
                    pShader->setUniform(name, numTex++);
	                glBindTexture(GL_TEXTURE_2D, diffuseTexture[i]->getID());
                }

                auto& specularTexture = pMaterial->fetchTextureOfType(ETexture::Specular);
                for (int i = 0; i < specularTexture.size(); ++i)
                {
                    glActiveTexture(GL_TEXTURE0 + numTex);
                    std::string name = std::format("tex_specular{}", i + 1);
                    pShader->setUniform(name, numTex++);
                    glBindTexture(GL_TEXTURE_2D, specularTexture[i]->getID());
                }

                auto& normalTexture = pMaterial->fetchTextureOfType(ETexture::Normal);
                for (int i = 0; i < normalTexture.size(); ++i)
                {
                    glActiveTexture(GL_TEXTURE0 + numTex);
                    std::string name = std::format("tex_normal{}", i + 1);
                    pShader->setUniform(name, numTex++);
                    glBindTexture(GL_TEXTURE_2D, normalTexture[i]->getID());
                }

                glActiveTexture(GL_TEXTURE0);

                pShader->setUniform("uUseTex", 1);
                pShader->setUniform("uUseNormalMap", 1);

                if (meshrdr->_bLit)
                {
                    for (int i = 0; i < m_SceneLights.size(); ++i)
                    {
                        pShader->setUniform("uLightPos", m_SceneLights[i]._Position);
                        pShader->setUniform("uLightColor", m_SceneLights[i]._Color);
                        pShader->setUniform("uLightIntensity", m_SceneLights[i]._Intensity);
                    }
                    pShader->setUniform("uViewPos", m_pSceneCamera->Position);
                }

                if (auto* light = m_Registry.try_get<PaleRdr::SCompPointLight>(id))
                {
                    pShader->setUniform("uLightColor", light->_Color);
                    pShader->setUniform("uIntensity", light->_Intensity);
                }

                glBindVertexArray(Mesh->getVAO());
                glDrawElements(Mesh->getElementType(), Mesh->getIndiceSize(), GL_UNSIGNED_INT, 0);
            }
        }
    }
    void CScene::__AfterRender()
    {
        m_SceneLights.clear();
    }
}