cmake_minimum_required(VERSION 3.20)  # 确保 CMake 版本兼容 VS2022
project(PaleEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 3rd party libraries ---
set(BUILD_SHARED_LIBS ON CACHE BOOL "glad Build shared libraries" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "glfw Build shared libraries" FORCE)
set(SPDLOG_BUILD_SHARED ON CACHE BOOL "spdlog Build shared libraries" FORCE)

foreach(lib IN ITEMS glad glfw glm imgui spdlog assimp EnTT)
    add_subdirectory(3rdparty/${lib})
    message(STATUS ">> Adding library: ${lib} from 3rdparty/${lib}")
    set_property(TARGET ${lib} PROPERTY FOLDER "3rdparty")
    set_target_properties(${lib} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endforeach()

# assimp
set_property(TARGET 
    UpdateAssimpLibsDebugSymbolsAndDLLs 
    zlibstatic 
    unit 
    uninstall
    update_mappings
    PROPERTY FOLDER "other")

# --- Ours ---
foreach(lib IN ITEMS PaleRenderer PaleEditor)
    add_subdirectory(${lib})
    set_target_properties(${lib} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

    set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "\\" "/" CURRENT_DIR_UNIX_STYLE ${ROOT_DIR})
    target_compile_definitions(${lib} PRIVATE ROOT_DIR="${ROOT_DIR}")
endforeach()

add_subdirectory(Assets)
