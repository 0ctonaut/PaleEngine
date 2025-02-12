#include "stdafx.h"
#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
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
        spdlog::error("glad failed!");
    }
    glfwSwapInterval(1); // Enable vsync

    // --- scene ---
    PaleRdr::CScene scene;
	PaleRdr::CPassOpenGL passModel(
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/model_blinnphong.vert",
		CPathManager::getInstance().getRootDir() / "Assets/Shaders/model_blinnphong.frag");
	
    PaleRdr::CModel ourModel(CPathManager::getInstance().getRootDir() / "Assets/Models/backpack/backpack.obj");
    
    auto& Registry = scene.fetchRegistry();
    entt::entity entity1 = scene.addEntity("backpack1");
    Registry.emplace<PaleRdr::SCompMeshRenderer>(entity1, ourModel.getMeshes(), passModel, true);

    entt::entity entity2 = scene.addEntity("backpack2");
    Registry.emplace<PaleRdr::SCompMeshRenderer>(entity2, ourModel.getMeshes(), passModel, true);

    PaleRdr::CPassOpenGL passLight(
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/light_point.vert",
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/light_point.frag");

    entt::entity light1 = scene.addEntity("light1", 
        glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1));
    Registry.emplace<PaleRdr::SCompMeshRenderer>(light1, PaleRdr::CModel::getCubeMeshes(), passLight, false);
    Registry.emplace<PaleRdr::SCompPointLight>(light1, glm::vec3(1, 1, 1), 1.0f);

    //entt::entity light2 = scene.addEntity("light2",
    //    glm::vec3(-5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1));
    //Registry.emplace<PaleRdr::SCompMeshRenderer>(light2, PaleRdr::CModel::getCubeMeshes(), passLight);
    //Registry.emplace<PaleRdr::SCompPointLight>(light2, glm::vec3(1, 1, 1), 1.0f);

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
    }
}


