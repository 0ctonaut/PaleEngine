cmake_minimum_required(VERSION 3.20)  # 确保 CMake 版本兼容 VS2022
project(PaleEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

# --- 3rd party libraries ---
add_subdirectory(3dparty/glad)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(3dparty/glfw)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "Build shared libraries" FORCE)
#set(GLM_BUILD_TESTS OFF CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(3dparty/glm)
add_subdirectory(3dparty/imgui)
set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(3dparty/spdlog)

# --- Ours ---
add_subdirectory(PaleRenderer)
add_subdirectory(PaleUI)
