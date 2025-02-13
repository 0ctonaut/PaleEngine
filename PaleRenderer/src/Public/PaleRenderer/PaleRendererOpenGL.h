#pragma once
//
#include "PaleRenderer/Core/Camera.h"
#include "PaleRenderer/Core/PathManager.h"
#include "PaleRenderer/Core/ModelLoader.h"
#include "PaleRenderer/Core/Application.h"

// --- ECS ---
#include "PaleRenderer/ECS/Scene.h"
#include "PaleRenderer/ECS/NameComp.h"
#include "PaleRenderer/ECS/TransformComp.h"
#include "PaleRenderer/ECS/MeshRendererComp.h"
#include "PaleRenderer/ECS/LightComp.h"

// --- OpenGL ---
#include "OpenGL/PassOpenGL.h"
#include "OpenGL/TextureOpenGL.h"
#include "OpenGL/WindowOpenGL.h"
#include "OpenGL/FrameBufferOpenGL.h"
// ------

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>