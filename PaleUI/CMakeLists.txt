project(PaleUI)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB SHADER_VERT "${ROOT_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${ROOT_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${ROOT_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${ROOT_DIR}/Assets/Images/*.jpg")

source_group("src" FILES ${HEADERS} ${SOURCES})
source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})

add_executable(PaleUI ${SOURCES} ${HEADERS} 
    ${SHADER_VERT} ${SHADER_FRAG}
    ${IMAGE_PNG} ${IMAGE_JPG}
)

# --- 3rdparty ---
target_link_libraries(PaleUI PRIVATE imgui)

# ---  ---
target_link_libraries(PaleUI PRIVATE PaleRenderer)

# --- macros ---
set(UI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\\" "/" CURRENT_DIR_UNIX_STYLE ${UI_ROOT})
target_compile_definitions(PaleUI PRIVATE UI_ROOT="${UI_ROOT}")