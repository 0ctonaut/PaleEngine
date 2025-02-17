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
    std::shared_ptr<PaleRdr::IShader> pShaderBlinnPhong = PaleRdr::IShader::Create(
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/model_blinnphong_normalmapping.vert",
		CPathManager::getInstance().getRootDir() / "Assets/Shaders/model_blinnphong_normalmapping.frag");
	
    std::shared_ptr<PaleRdr::IShader> pShaderLight = PaleRdr::IShader::Create(
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/light_point.vert",
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/light_point.frag");

    std::shared_ptr<PaleRdr::IShader> pShaderPBR = PaleRdr::IShader::Create(
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/model_pbr.vert",
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/model_pbr.frag");

    std::shared_ptr<PaleRdr::IMaterial> pMaterialRustediron = PaleRdr::IMaterial::Create(pShaderPBR);
    auto addTexture = [&pMaterialRustediron](const std::filesystem::path& vPath, PaleRdr::ETexture vType) {
        std::shared_ptr<PaleRdr::ITexture> texture = PaleRdr::ITexture::Create(vPath, vType);
        pMaterialRustediron->addTextureOfType(vType, texture);
        };

    addTexture(CPathManager::getInstance().getRootDir() / "Assets/Models/rustediron/albedo.png", PaleRdr::ETexture::Albedo);
    addTexture(CPathManager::getInstance().getRootDir() / "Assets/Models/rustediron/normal.png", PaleRdr::ETexture::Normal);
    addTexture(CPathManager::getInstance().getRootDir() / "Assets/Models/rustediron/metallic.png", PaleRdr::ETexture::Metallic);
    addTexture(CPathManager::getInstance().getRootDir() / "Assets/Models/rustediron/roughness.png", PaleRdr::ETexture::Roughness);
    addTexture(CPathManager::getInstance().getRootDir() / "Assets/Models/rustediron/ao.png", PaleRdr::ETexture::AO);

    // ---
    
    //PaleRdr::CModelLoader ourModel(CPathManager::getInstance().getRootDir() / "Assets/Models/backpack/backpack.obj");
    //entt::entity entity1 = scene.addEntity("backpack",
    //    glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    //
    //std::unordered_map<int, std::shared_ptr<PaleRdr::IMaterial>> Materials = ourModel.getMaterials();
    //for (auto& mat : Materials)
    //{
    //    mat.second->setShader(pShaderBlinnPhong);
    //}

    //Registry.emplace<PaleRdr::SCompMeshRenderer>(entity1, 
    //    ourModel.getMeshes(), Materials, true);

    // --- 

    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            entt::entity entity = scene.addEntity("ball",
                glm::vec3(i * 3, j * 3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

            Registry.emplace<PaleRdr::SCompMeshRenderer>(entity,
                PaleRdr::Sphere::getMeshes(), pMaterialRustediron, true);
        }
    }

    // ---

    entt::entity light1 = scene.addEntity("light1", 
        glm::vec3(5, 5, 10), glm::vec3(0, 0, 0), glm::vec3(0.05, 0.05, 0.05));
    Registry.emplace<PaleRdr::SCompMeshRenderer>(light1, 
        PaleRdr::Sphere::getMeshes(), pShaderLight, false);
    Registry.emplace<PaleRdr::SCompPointLight>(light1, glm::vec3(1, 1, 1), 1.0f);


    entt::entity light2 = scene.addEntity("light2",
        glm::vec3(-5, 5, 10), glm::vec3(0, 0, 0), glm::vec3(0.05, 0.05, 0.05));
    Registry.emplace<PaleRdr::SCompMeshRenderer>(light2,
        PaleRdr::Sphere::getMeshes(), pShaderLight, false);
    Registry.emplace<PaleRdr::SCompPointLight>(light2, glm::vec3(1, 1, 1), 1.0f);


    entt::entity light3 = scene.addEntity("light3",
        glm::vec3(-5, -5, 10), glm::vec3(0, 0, 0), glm::vec3(0.05, 0.05, 0.05));
    Registry.emplace<PaleRdr::SCompMeshRenderer>(light3,
        PaleRdr::Sphere::getMeshes(), pShaderLight, false);
    Registry.emplace<PaleRdr::SCompPointLight>(light3, glm::vec3(1, 1, 1), 1.0f);


    entt::entity light4 = scene.addEntity("light4",
        glm::vec3(5, -5, 10), glm::vec3(0, 0, 0), glm::vec3(0.05, 0.05, 0.05));
    Registry.emplace<PaleRdr::SCompMeshRenderer>(light4,
        PaleRdr::Sphere::getMeshes(), pShaderLight, false);
    Registry.emplace<PaleRdr::SCompPointLight>(light4, glm::vec3(1, 1, 1), 1.0f);

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


