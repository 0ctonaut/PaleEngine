#include "stdafx.h"
#include "UIManager.h"

float lastX = 1920 / 2.0f;
float lastY = 1080 / 2.0f;
bool firstMouse = true;
bool bMouseRightButtonPress = false;
void processInput(GLFWwindow* window, PaleRdr::CCamera* vCam, float vDeltaTime);

// Main code
int main(int, char**)
{
    spdlog::set_level(spdlog::level::trace);
    PaleRdr::CApplication app;
    GLFWwindow* pWindow = app.fetchWindow()->fetchWindowPtr();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        PALE_EDITOR_ERROR("glad Loading failed!");
    }
    glfwSwapInterval(1); // Enable vsync

    // --- scene ---
    PaleRdr::CScene scene;
    auto& Registry = scene.fetchRegistry();
    auto addTexture = [](
        const std::shared_ptr<PaleRdr::IMaterial>& vMat,
        const std::filesystem::path& vPath,
        PaleRdr::ETexture vType) {
            std::shared_ptr<PaleRdr::ITexture> texture = PaleRdr::ITexture::Create(vPath, vType);
            vMat->addTextureOfType(vType, texture);
        };

    // --- skybox ---

	std::vector<std::filesystem::path> skyboxFaces
	{
		CPathManager::getInstance().getRootDir() / "Assets/textures/skybox/right.jpg",
		CPathManager::getInstance().getRootDir() / "Assets/textures/skybox/left.jpg",
		CPathManager::getInstance().getRootDir() / "Assets/textures/skybox/top.jpg",
		CPathManager::getInstance().getRootDir() / "Assets/textures/skybox/bottom.jpg",
		CPathManager::getInstance().getRootDir() / "Assets/textures/skybox/front.jpg",
		CPathManager::getInstance().getRootDir() / "Assets/textures/skybox/back.jpg"
	};

	std::shared_ptr<PaleRdr::ITexture> pTextureOldSkybox = PaleRdr::ITexture::Create(skyboxFaces, PaleRdr::ETexture::Skybox);
    std::shared_ptr<PaleRdr::ITexture> pTextureSkybox = PaleRdr::ITexture::Create(
        CPathManager::getInstance().getRootDir() / "Assets/textures/hdr/newport_loft.hdr",
        PaleRdr::ETexture::Skybox, GL_RGB32F);
    std::shared_ptr<PaleRdr::ITexture> pTextureIrradiance = PaleRdr::ITexture::CreateIrradianceMap(
        pTextureSkybox, 32, 32
    );

    std::shared_ptr<PaleRdr::ITexture> pTexturePrefilterMap = PaleRdr::ITexture::CreatePreFilterMap(
        pTextureSkybox, 128, 128
    );

    std::shared_ptr<PaleRdr::ITexture> pTextureBRDFLUT = PaleRdr::ITexture::CreateBRDFLUT(
        512, 512
    );

    {
		entt::entity entity = scene.addEntity("skybox",
			glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

        std::shared_ptr<PaleRdr::IShader> pShaderSkybox = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/skybox.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/skybox/skybox.frag");
		Registry.emplace<PaleRdr::SCompSkybox>(entity, PaleRdr::Cube::getMeshes()[0], pShaderSkybox, 
            pTextureSkybox, pTextureIrradiance, pTexturePrefilterMap, pTextureBRDFLUT);
	}

    // --- meshes ---
    float gap = 1.5f;

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Test/model.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Test/model.frag");
        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        entt::entity entity = scene.addEntity("sphere", gap * glm::vec3(-2, 3, -2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Test/model_normal.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Test/model_normal.frag");

        entt::entity entity = scene.addEntity("sphere_normal", gap * glm::vec3(-2, 3, -1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pShader, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_skybox_refract.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_skybox_refract.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        pMaterial->addTextureOfType(PaleRdr::ETexture::Skybox, pTextureSkybox);

        entt::entity entity = scene.addEntity("sphere_skybox_refract", gap * glm::vec3(-2, 3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_skybox_reflect.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_skybox_reflect.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        pMaterial->addTextureOfType(PaleRdr::ETexture::Skybox, pTextureSkybox);

        entt::entity entity = scene.addEntity("sphere_skybox_reflect", gap * glm::vec3(-2, 3, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }
    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/rusted_iron/albedo.png", PaleRdr::ETexture::Albedo);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/rusted_iron/normal.png", PaleRdr::ETexture::Normal);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/rusted_iron/metallic.png", PaleRdr::ETexture::Metallic);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/rusted_iron/roughness.png", PaleRdr::ETexture::Roughness);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/rusted_iron/ao.png", PaleRdr::ETexture::AO);
        pMaterial->addTextureOfType(PaleRdr::ETexture::IrradianceMap, pTextureIrradiance);
        pMaterial->addTextureOfType(PaleRdr::ETexture::PreFilterMap, pTexturePrefilterMap);
        pMaterial->addTextureOfType(PaleRdr::ETexture::BRDFLUT, pTextureBRDFLUT);

		entt::entity entity = scene.addEntity("sphere_rust", gap * glm::vec3(-2, 2, -2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

		Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/gold/albedo.png", PaleRdr::ETexture::Albedo);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/gold/normal.png", PaleRdr::ETexture::Normal);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/gold/metallic.png", PaleRdr::ETexture::Metallic);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/gold/roughness.png", PaleRdr::ETexture::Roughness);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/gold/ao.png", PaleRdr::ETexture::AO);
        pMaterial->addTextureOfType(PaleRdr::ETexture::IrradianceMap, pTextureIrradiance);
        pMaterial->addTextureOfType(PaleRdr::ETexture::PreFilterMap, pTexturePrefilterMap);
        pMaterial->addTextureOfType(PaleRdr::ETexture::BRDFLUT, pTextureBRDFLUT);

        entt::entity entity = scene.addEntity("sphere_gold", gap * glm::vec3(-2, 2, -1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/grass/albedo.png", PaleRdr::ETexture::Albedo);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/grass/normal.png", PaleRdr::ETexture::Normal);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/grass/metallic.png", PaleRdr::ETexture::Metallic);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/grass/roughness.png", PaleRdr::ETexture::Roughness);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/grass/ao.png", PaleRdr::ETexture::AO);
        pMaterial->addTextureOfType(PaleRdr::ETexture::IrradianceMap, pTextureIrradiance);
        pMaterial->addTextureOfType(PaleRdr::ETexture::PreFilterMap, pTexturePrefilterMap);
        pMaterial->addTextureOfType(PaleRdr::ETexture::BRDFLUT, pTextureBRDFLUT);

        entt::entity entity = scene.addEntity("sphere_grass", gap * glm::vec3(-2, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/plastic/albedo.png", PaleRdr::ETexture::Albedo);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/plastic/normal.png", PaleRdr::ETexture::Normal);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/plastic/metallic.png", PaleRdr::ETexture::Metallic);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/plastic/roughness.png", PaleRdr::ETexture::Roughness);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/plastic/ao.png", PaleRdr::ETexture::AO);
        pMaterial->addTextureOfType(PaleRdr::ETexture::IrradianceMap, pTextureIrradiance);
        pMaterial->addTextureOfType(PaleRdr::ETexture::PreFilterMap, pTexturePrefilterMap);
        pMaterial->addTextureOfType(PaleRdr::ETexture::BRDFLUT, pTextureBRDFLUT);

        entt::entity entity = scene.addEntity("sphere_plastic", gap * glm::vec3(-2, 2, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }

    {
        std::shared_ptr<PaleRdr::IShader> pShader = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/Model/model_pbr.frag");

        std::shared_ptr<PaleRdr::IMaterial> pMaterial = PaleRdr::IMaterial::Create(pShader);

        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/wall/albedo.png", PaleRdr::ETexture::Albedo);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/wall/normal.png", PaleRdr::ETexture::Normal);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/wall/metallic.png", PaleRdr::ETexture::Metallic);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/wall/roughness.png", PaleRdr::ETexture::Roughness);
        addTexture(pMaterial, CPathManager::getInstance().getRootDir() / "Assets/textures/pbr/wall/ao.png", PaleRdr::ETexture::AO);
        pMaterial->addTextureOfType(PaleRdr::ETexture::IrradianceMap, pTextureIrradiance);
        pMaterial->addTextureOfType(PaleRdr::ETexture::PreFilterMap, pTexturePrefilterMap);
        pMaterial->addTextureOfType(PaleRdr::ETexture::BRDFLUT, pTextureBRDFLUT);

        entt::entity entity = scene.addEntity("sphere_wall", gap * glm::vec3(-2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

        Registry.emplace<PaleRdr::SCompMeshRenderer>(entity, PaleRdr::Sphere::getMeshes(), pMaterial, true);
    }


    // --- lights ---
    {
        std::shared_ptr<PaleRdr::IShader> pShaderLight = PaleRdr::IShader::Create(
            CPathManager::getInstance().getRootDir() / "Assets/shaders/light_point.vert",
            CPathManager::getInstance().getRootDir() / "Assets/shaders/light_point.frag");

        std::vector<glm::vec3> lightPositions = {
            glm::vec3(5, 5, 10),
            glm::vec3(-5, 5, 10),
            glm::vec3(-5, -5, 10),
            glm::vec3(5, -5, 10)
        };

        for (int i = 0; i < lightPositions.size(); ++i)
        {
            entt::entity light = scene.addEntity(std::format("light{}", i), lightPositions[i], glm::vec3(0, 0, 0), glm::vec3(0.05, 0.05, 0.05));
            Registry.emplace<PaleRdr::SCompMeshRenderer>(light, PaleRdr::Sphere::getMeshes(), pShaderLight, false);
            Registry.emplace<PaleRdr::SCompPointLight>(light, glm::vec3(1, 1, 1), 1.0f);
        }
    }

    // --- ---

    PaleUI::CUIManager UIManager;
    UIManager.OnUISetup(pWindow);
    UIManager.EventViewportResize.subscribe([&app, &scene](unsigned int vWidth, unsigned int vHeight) {
        app.OnViewportResize(vWidth, vHeight);
        scene.fetchSceneCam()->OnViewportResize(vWidth, vHeight);
        });

#pragma region glfw callback
    glfwSetFramebufferSizeCallback(pWindow, [](GLFWwindow* window, int width, int height) {
        // viewport resizing is up to imgui window
        // glViewport(0, 0, width, height);
        spdlog::debug("window resize to {} {}", width, height);
        });

    glfwSetWindowUserPointer(pWindow, scene.fetchSceneCam());
    glfwSetCursorPosCallback(pWindow, [](GLFWwindow* window, double xposIn, double yposIn) {
        ImGui_ImplGlfw_CursorPosCallback(window, xposIn, yposIn);
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;


        if (bMouseRightButtonPress)
        {
            auto* cam = static_cast<PaleRdr::CCamera*>(glfwGetWindowUserPointer(window));
            cam->ProcessMouseMovement(xoffset, yoffset);
        }
        });

    glfwSetScrollCallback(pWindow, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            auto* cam = static_cast<PaleRdr::CCamera*>(glfwGetWindowUserPointer(window));
            cam->ProcessMouseScroll(static_cast<float>(yoffset));
        });
#pragma endregion glfw callback

    ImGuiIO& io = ImGui::GetIO();
    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();
        app.OnRender(io.Framerate);

        UIManager.OnUIBeginFrame();

        processInput(pWindow, scene.fetchSceneCam(), app.getDeltaTime());
        scene.OnRender(app.getFrameBuffer());

        UIManager.OnUIRender(scene, app.getFrameBuffer());
        UIManager.OnUIEndFrame();

        glfwSwapBuffers(pWindow);
    }

    UIManager.OnUIDestroy();
    return 0;
}

void processInput(GLFWwindow* window, PaleRdr::CCamera* vCam, float vDeltaTime)
{
    bMouseRightButtonPress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    
    if (bMouseRightButtonPress)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::FORWARD, vDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::BACKWARD, vDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::LEFT, vDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::RIGHT, vDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::UP, vDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::DOWN, vDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            vCam->SpeedAcc = 3.0;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            vCam->SpeedAcc = 1.0;
    }
}


