#pragma once
//
#include "PaleRenderer/Core/Camera.h"
#include "PaleRenderer/Core/PathManager.h"
#include "PaleRenderer/Core/Model.h"
#include "PaleRenderer/Core/Application.h"

// --- ECS ---
#include "ECS/Scene.h"
#include "ECS/NameComp.h"
#include "ECS/TransformComp.h"
#include "ECS/MeshRendererComp.h"

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