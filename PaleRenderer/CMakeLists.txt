project(PaleRenderer)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB SHADER_VERT "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images/*.jpg")
file(GLOB OPENGL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/OpenGL/*.h")
file(GLOB OPENGL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/OpenGL/*.cpp")
file(GLOB COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.h")
file(GLOB COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.cpp")
file(GLOB CORE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.h")
file(GLOB CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp")

source_group("src" FILES ${HEADERS} ${SOURCES})
source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})
source_group("OpenGL" FILES ${OPENGL_HEADERS} ${OPENGL_SOURCES})
source_group("Common" FILES ${COMMON_HEADERS} ${COMMON_SOURCES})
source_group("Core" FILES ${CORE_HEADERS} ${CORE_SOURCES})

add_library(PaleRenderer SHARED ${SOURCES} ${HEADERS} 
    ${SHADER_VERT} ${SHADER_FRAG}
    ${IMAGE_PNG} ${IMAGE_JPG}
)

target_include_directories(PaleRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties( PaleRenderer PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

# --- 3rdparty ---
target_link_libraries(PaleRenderer PUBLIC glad)
target_link_libraries(PaleRenderer PUBLIC glfw)
target_link_libraries(PaleRenderer PUBLIC glm)
target_link_libraries(PaleRenderer PUBLIC spdlog)
target_link_libraries(PaleRenderer PUBLIC assimp)

# --- macros ---
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