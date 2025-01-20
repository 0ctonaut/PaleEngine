project(PaleRenderer)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB SHADER_VERT "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images/*.jpg")

source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})

add_executable(PaleRenderer ${SOURCES} ${HEADERS} 
    ${SHADER_VERT} ${SHADER_FRAG}
    ${IMAGE_PNG} ${IMAGE_JPG}
)
# add_library(PaleRenderer SHARED ${SOURCES} ${HEADERS})

target_include_directories(PaleRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


# --- 3dparty ---
target_link_libraries(PaleRenderer PRIVATE glad)
target_link_libraries(PaleRenderer PRIVATE glm)

# --- macros ---
target_compile_definitions(PaleRenderer PRIVATE DLL_EXPORT)
set(RENDERER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\\" "/" CURRENT_DIR_UNIX_STYLE ${RENDERER_ROOT})
target_compile_definitions(PaleRenderer PRIVATE RENDERER_ROOT="${RENDERER_ROOT}")