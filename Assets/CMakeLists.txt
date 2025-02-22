project(AssetsPreview)

file(GLOB_RECURSE SHADER_VERT "${ROOT_DIR}/Assets/shaders/*.vert")
file(GLOB_RECURSE SHADER_FRAG "${ROOT_DIR}/Assets/shaders/*.frag")

set(ASSET_FILES ${SHADER_VERT} ${SHADER_FRAG})

source_group("shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})

add_custom_target(AssetsPreview SOURCES ${ASSET_FILES})