project(AssetsPreview)

file(GLOB SHADER_VERT "${ROOT_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${ROOT_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${ROOT_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${ROOT_DIR}/Assets/Images/*.jpg")
file(GLOB_RECURSE MODELS "${ROOT_DIR}/Assets/Models/*")

set(ASSET_FILES ${SHADER_VERT} ${SHADER_FRAG} ${IMAGE_PNG} ${IMAGE_JPG} ${MODELS})

source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})
source_group("Models" FILES ${MODELS})

add_custom_target(AssetsPreview SOURCES ${ASSET_FILES})