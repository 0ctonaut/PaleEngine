#pragma once
// --- Core ---
#include "PaleRenderer/Core/Log.h"
#include "PaleRenderer/Core/Camera.h"
#include "PaleRenderer/Core/PathManager.h"
#include "PaleRenderer/Core/Application.h"

// --- Mesh ---
#include "PaleRenderer/Mesh/Sphere.h"
#include "PaleRenderer/Mesh/Cube.h"
#include "PaleRenderer/Mesh/ModelLoader.h"
#include "PaleRenderer/Mesh/Mesh.h"

// --- Material ---
#include "PaleRenderer/Material/Shader.h"
#include "PaleRenderer/Material/Texture.h"

// --- ECS ---
#include "PaleRenderer/ECS/Scene.h"
#include "PaleRenderer/ECS/NameComp.h"
#include "PaleRenderer/ECS/TransformComp.h"
#include "PaleRenderer/ECS/MeshRendererComp.h"
#include "PaleRenderer/ECS/LightComp.h"
#include "PaleRenderer/ECS/CompSkybox.h"

// --- OpenGL ---
#include "PaleRenderer/OpenGL/WindowOpenGL.h"
#include "PaleRenderer/OpenGL/FrameBufferOpenGL.h"
