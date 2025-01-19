cmake_minimum_required(VERSION 3.20)  # 确保 CMake 版本兼容 VS2022
project(PaleEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 3rd party libraries ---
#add_subdirectory(3dparty/GLFW)
add_subdirectory(3dparty/glad)

# --- Ours ---
add_subdirectory(PaleRenderer)
## add_subdirectory(PaleUI)
