#include "stdafx.h"
#include "Scene.h"
#include "PaleRenderer/ECS/NameComp.h"
#include "PaleRenderer/ECS/TransformComp.h"
#include "PaleRenderer/ECS/MeshRendererComp.h"
#include "PaleRenderer/ECS/LightComp.h"
#include "PaleRenderer/ECS/CompSkybox.h"

#include "PaleRenderer/Core/Application.h"
#include "PaleRenderer/Core/PathManager.h"
#include "PaleRenderer/Mesh/Mesh.h"
#include "PaleRenderer/Mesh/Sphere.h"
#include "PaleRenderer/Material/Material.h"
#include "PaleRenderer/OpenGL/FrameBufferOpenGL.h"


namespace PaleRdr
{
    CScene::CScene()
    {
        glEnable(GL_DEPTH_TEST);
        m_BackgroundColor = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
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
        __OnRenderSkybox();
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

    void CScene::__OnRenderSkybox()
    {
        for (auto& id : m_Entities)
        {
            if (auto* skybox = m_Registry.try_get<PaleRdr::SCompSkybox>(id))
            {
                std::shared_ptr<IMesh>& pMesh = skybox->_SkyboxMesh;
                std::shared_ptr<ITexture>& pTexture = skybox->_SkyboxTexture;
                std::shared_ptr<IShader>& pShader = skybox->_SkyboxShader;

                pShader->use();
                glm::mat4 uView = glm::mat4(glm::mat3(m_pSceneCamera->getViewMatrix()));
                glm::mat4 uProject = m_pSceneCamera->getProjectionMatrix();
                auto* trans = m_Registry.try_get<PaleRdr::SCompTransform>(id);
                glm::mat4 uModel = trans->getTransfrom();

                pShader->setUniform("uView", uView);
                pShader->setUniform("uProjection", uProject);
                pShader->setUniform("uModel", uModel);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, pTexture->getID());
                pShader->setUniform("skybox", static_cast<int>(0));
                glBindVertexArray(pMesh->getVAO());
                glDepthFunc(GL_LEQUAL);
                glDrawElements(pMesh->getElementType(), pMesh->getIndiceSize(), GL_UNSIGNED_INT, 0);
                glDepthFunc(GL_LESS);
                glBindVertexArray(0);
            }
        }
    }

    void CScene::__OnRenderLight()
    {
        for (auto& id : m_Entities)
        {
            if (auto* light = m_Registry.try_get<PaleRdr::SCompPointLight>(id))
            {
                auto* trans = m_Registry.try_get<PaleRdr::SCompTransform>(id);
                m_SceneLights.push_back({trans->_Position, light->_Color * 100.0f, light->_Intensity});
            }
        }
    }

    void CScene::__OnRenderMeshRdr()
    {
        for (auto& id : m_Entities)
        {
            auto* name = m_Registry.try_get<PaleRdr::SCompName>(id);
            auto* trans = m_Registry.try_get<PaleRdr::SCompTransform>(id);
            PALE_RDR_ASSERT(trans, "Entities must have a Transform!");
            auto* meshrdr = m_Registry.try_get<PaleRdr::SCompMeshRenderer>(id);
            if (meshrdr == nullptr)
            {
                continue;
            }

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
                auto bindTextures = [&numTex, &pShader, &pMaterial](ETexture vType, const std::string& vUniformName) {
                        if (pMaterial->hasTextureOfType(vType))
                        {
                            auto& textures = pMaterial->fetchTextureOfType(vType);
                            for (size_t i = 0; i < textures.size(); ++i)
                            {
                                glActiveTexture(GL_TEXTURE0 + numTex);
                                std::string name = std::format("{}{}", vUniformName, i + 1);
                                pShader->setUniform(name, numTex++);
                                if (vType == ETexture::Skybox ||
                                    vType == ETexture::IrradianceMap ||
                                    vType == ETexture::PreFilterMap
                                    )
                                {
                                    glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i]->getID());
                                }
                                else
                                {
                                    glBindTexture(GL_TEXTURE_2D, textures[i]->getID());
                                }
                            }
                        }
                    };

                bindTextures(ETexture::Diffuse, "tex_diffuse");
                bindTextures(ETexture::Specular, "tex_specular");
                bindTextures(ETexture::Albedo, "tex_albedo");
                bindTextures(ETexture::Normal, "tex_normal");
                bindTextures(ETexture::Metallic, "tex_metallic");
                bindTextures(ETexture::Roughness, "tex_roughness");
                bindTextures(ETexture::AO, "tex_ao");
                bindTextures(ETexture::Skybox, "tex_skybox");
                bindTextures(ETexture::IrradianceMap, "tex_irradiance");
                bindTextures(ETexture::PreFilterMap, "tex_prefilter");
                bindTextures(ETexture::BRDFLUT, "tex_brdflut");

                glActiveTexture(GL_TEXTURE0);

                pShader->setUniform("uUseDiffuse", pMaterial->hasTextureOfType(ETexture::Diffuse));
                pShader->setUniform("uUseSpecular", pMaterial->hasTextureOfType(ETexture::Specular));
                pShader->setUniform("uUseAlbedo", pMaterial->hasTextureOfType(ETexture::Albedo));
                pShader->setUniform("uUseNormal", pMaterial->hasTextureOfType(ETexture::Normal));
                pShader->setUniform("uUseMetallic", pMaterial->hasTextureOfType(ETexture::Metallic));
                pShader->setUniform("uUseRoughness", pMaterial->hasTextureOfType(ETexture::Roughness));
                pShader->setUniform("uUseAO", pMaterial->hasTextureOfType(ETexture::AO));

                if (meshrdr->_bLit)
                {
                    for (int i = 0; i < m_SceneLights.size(); ++i)
                    {
                        pShader->setUniform("uLightPos[" + std::to_string(i) + "]", m_SceneLights[i]._Position);
                        pShader->setUniform("uLightColor[" + std::to_string(i) + "]", m_SceneLights[i]._Color);
                        pShader->setUniform("uLightIntensity[" + std::to_string(i) + "]", m_SceneLights[i]._Intensity);
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