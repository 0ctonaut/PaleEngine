project(PaleRenderer)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB SHADER_VERT "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images/*.jpg")

source_group("src" FILES ${HEADERS} ${SOURCES})
source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})

add_library(PaleRenderer SHARED ${SOURCES} ${HEADERS} 
    ${SHADER_VERT} ${SHADER_FRAG}
    ${IMAGE_PNG} ${IMAGE_JPG}
)

target_include_directories(PaleRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(PaleRenderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h)

# --- 3dparty ---
target_link_libraries(PaleRenderer PUBLIC glad)
target_link_libraries(PaleRenderer PUBLIC glfw)
target_link_libraries(PaleRenderer PUBLIC glm)
target_link_libraries(PaleRenderer PUBLIC spdlog)

# --- macros ---
get_target_property(LIB_TYPE PaleRenderer TYPE)
if (LIB_TYPE STREQUAL "STATIC_LIBRARY")
    message(STATUS "Target PaleRenderer is a STATIC library.")
elseif (LIB_TYPE STREQUAL "SHARED_LIBRARY")
    message(STATUS "Target PaleRenderer is a SHARED library.")
    target_compile_definitions(PaleRenderer PRIVATE DYNAMIC_LINK)
    target_compile_definitions(PaleRenderer PRIVATE DLL_EXPORT)
elseif (LIB_TYPE STREQUAL "EXECUTABLE")
    message(STATUS "Target PaleRenderer is an EXECUTABLE.")
else()
    message(STATUS "Target PaleRenderer is of type ${MY_TARGET_TYPE}.")
endif()
set(RENDERER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\\" "/" CURRENT_DIR_UNIX_STYLE ${RENDERER_ROOT})
target_compile_definitions(PaleRenderer PRIVATE RENDERER_ROOT="${RENDERER_ROOT}")