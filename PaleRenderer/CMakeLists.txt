project(PaleRenderer)

file(GLOB_RECURSE HEADERS_PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/*.h")
file(GLOB_RECURSE SOURCES_PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/*.cpp")
file(GLOB_RECURSE HEADERS_PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Private/PaleRenderer/*.h")
file(GLOB_RECURSE SOURCES_PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Private/PaleRenderer/*.cpp")

# Public
file(GLOB OPENGL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/OpenGL/*.h")
file(GLOB OPENGL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/OpenGL/*.cpp")
file(GLOB COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Common/*.h")
file(GLOB COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Common/*.cpp")
file(GLOB CORE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Core/*.h")
file(GLOB CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Core/*.cpp")
file(GLOB ECS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/ECS/*.h")
file(GLOB ECS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/ECS/*.cpp")
file(GLOB MESH_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Mesh/*.h")
file(GLOB MESH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Mesh/*.cpp")
file(GLOB MATERIAL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Material/*.h")
file(GLOB MATERIAL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/PaleRenderer/Material/*.cpp")

# Private
file(GLOB OPENGL_HEADERS_PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Private/PaleRenderer/OpenGL/*.h")
file(GLOB OPENGL_SOURCES_PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Private/PaleRenderer/OpenGL/*.cpp")

source_group("src" FILES ${HEADERS_PUBLIC} ${SOURCES_PUBLIC} ${HEADERS_PRIVATE} ${SOURCES_PRIVATE})
source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})
source_group("OpenGL" FILES ${OPENGL_HEADERS} ${OPENGL_SOURCES} ${OPENGL_HEADERS_PRIVATE} ${OPENGL_SOURCES_PRIVATE})
source_group("Common" FILES ${COMMON_HEADERS} ${COMMON_SOURCES})
source_group("Core" FILES ${CORE_HEADERS} ${CORE_SOURCES})
source_group("ECS" FILES ${ECS_HEADERS} ${ECS_SOURCES})
source_group("Mesh" FILES ${MESH_HEADERS} ${MESH_SOURCES})
source_group("Material" FILES ${MATERIAL_HEADERS} ${MATERIAL_SOURCES})

add_library(PaleRenderer SHARED ${HEADERS_PUBLIC} ${SOURCES_PUBLIC} ${HEADERS_PRIVATE} ${SOURCES_PRIVATE} 
    ${SHADER_VERT} ${SHADER_FRAG}
    ${IMAGE_PNG} ${IMAGE_JPG}
)

target_include_directories(PaleRenderer 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Private)

set_target_properties( PaleRenderer PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

# --- 3rdparty ---
target_link_libraries(PaleRenderer PUBLIC glad)
target_link_libraries(PaleRenderer PUBLIC glfw)
target_link_libraries(PaleRenderer PUBLIC glm)
target_link_libraries(PaleRenderer PUBLIC spdlog)
target_link_libraries(PaleRenderer PUBLIC assimp)
target_link_libraries(PaleRenderer PUBLIC EnTT)

# --- macros ---
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(PaleRenderer PUBLIC PALE_PLATFORM_WINDOWS)
endif()

get_target_property(LIB_TYPE PaleRenderer TYPE)
if (LIB_TYPE STREQUAL "STATIC_LIBRARY")
    message(STATUS "PaleRenderer: STATIC library.")
elseif (LIB_TYPE STREQUAL "SHARED_LIBRARY")
    message(STATUS "PaleRenderer: SHARED library.")
    target_compile_definitions(PaleRenderer PRIVATE DYNAMIC_LINK)
    target_compile_definitions(PaleRenderer PRIVATE DLL_EXPORT)
elseif (LIB_TYPE STREQUAL "EXECUTABLE")
    message(STATUS "PaleRenderer: EXECUTABLE.")
else()
    message(STATUS "PaleRenderer:  ${MY_TARGET_TYPE}.")
endif()
set(RENDERER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\\" "/" CURRENT_DIR_UNIX_STYLE ${RENDERER_ROOT})
target_compile_definitions(PaleRenderer PRIVATE RENDERER_ROOT="${RENDERER_ROOT}")