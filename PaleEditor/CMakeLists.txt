project(PaleEditor)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB PANELS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Panels/*.h")
file(GLOB PANELS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Panels/*.cpp")

file(GLOB SHADER_VERT "${ROOT_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${ROOT_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${ROOT_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${ROOT_DIR}/Assets/Images/*.jpg")

source_group("src" FILES ${HEADERS} ${SOURCES})
source_group("Panels" FILES ${PANELS_HEADERS} ${PANELS_SOURCES})
source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})

add_executable(PaleEditor ${SOURCES} ${HEADERS} 
    ${SHADER_VERT} ${SHADER_FRAG}
    ${IMAGE_PNG} ${IMAGE_JPG}
)

target_include_directories(PaleEditor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(PaleEditor PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")

# --- 3rdparty ---
target_link_libraries(PaleEditor PRIVATE imgui)

# ---  ---
target_link_libraries(PaleEditor PRIVATE PaleRenderer)

# --- macros ---
set(UI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\\" "/" CURRENT_DIR_UNIX_STYLE ${UI_ROOT})
target_compile_definitions(PaleEditor PRIVATE UI_ROOT="${UI_ROOT}")