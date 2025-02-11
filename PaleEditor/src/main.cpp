#include "stdafx.h"
#define GL_SILENCE_DEPRECATION
#include <PaleRenderer/PaleRendererOpenGL.h>
#include <GLFW/glfw3.h>
#include "UIManager.h"

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float lastX = 1920 / 2.0f;
float lastY = 1080 / 2.0f;
bool firstMouse = true;
bool bMouseRightButtonPress = false;
void processInput(GLFWwindow* window, PaleRdr::CCamera* vCam);

// Main code
int main(int, char**)
{
    spdlog::set_level(spdlog::level::trace);
    GLFWwindow* pWindow = PaleRdr::CApplication::getInstance().fetchWindow()->fetchWindowPtr();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("glad failed!");
    }
    glfwSwapInterval(1); // Enable vsync

	PaleRdr::CPassOpenGL pass1(
        CPathManager::getInstance().getRootDir() / "Assets/Shaders/model.vert",
		CPathManager::getInstance().getRootDir() / "Assets/Shaders/model.frag");
	
    PaleRdr::CModel ourModel(CPathManager::getInstance().getRootDir() / "Assets/Models/backpack/backpack.obj");
    PaleRdr::CScene scene;
    auto& Registry = scene.fetchRegistry();
    entt::entity entity1 = scene.addEntity("backpack1");
    Registry.emplace<PaleRdr::SCompMeshRenderer>(entity1, ourModel.getMeshes(), pass1);

    entt::entity entity2 = scene.addEntity("backpack2");
    Registry.emplace<PaleRdr::SCompMeshRenderer>(entity2, ourModel.getMeshes(), pass1);

    PaleUI::CUIManager UIManager;
    UIManager.OnUISetup(pWindow);
    UIManager.EventViewportResize.subscribe([](unsigned int vWidth, unsigned int vHeight) {
        PaleRdr::CApplication::getInstance().OnViewportResize(vWidth, vHeight);
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
        PaleRdr::CApplication::getInstance().OnRender(io.Framerate);

        UIManager.OnUIBeginFrame();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(pWindow, scene.fetchSceneCam());
        
        PaleRdr::CApplication::getInstance().getFrameBuffer()->bind();
        scene.OnRender();
        PaleRdr::CApplication::getInstance().getFrameBuffer()->unbind();

        UIManager.OnUIRender(scene);
        UIManager.OnUIEndFrame();

        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();
    UIManager.OnUIDestroy();
    return 0;
}

void processInput(GLFWwindow* window, PaleRdr::CCamera* vCam)
{
    bMouseRightButtonPress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    
    if (bMouseRightButtonPress)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            vCam->ProcessKeyboard(PaleRdr::ECameraMove::DOWN, deltaTime);
    }
}


