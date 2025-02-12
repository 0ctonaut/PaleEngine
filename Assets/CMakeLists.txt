project(AssetsPreview)

file(GLOB SHADER_VERT "${ROOT_DIR}/Assets/Shaders/*.vert")
file(GLOB SHADER_FRAG "${ROOT_DIR}/Assets/Shaders/*.frag")
file(GLOB IMAGE_PNG "${ROOT_DIR}/Assets/Images/*.png")
file(GLOB IMAGE_JPG "${ROOT_DIR}/Assets/Images/*.jpg")

set(ASSET_FILES ${SHADER_VERT} ${SHADER_FRAG} ${IMAGE_PNG} ${IMAGE_JPG})

source_group("Shaders" FILES ${SHADER_VERT} ${SHADER_FRAG})
source_group("Images" FILES ${IMAGE_PNG} ${IMAGE_JPG})

add_custom_target(AssetsPreview SOURCES ${ASSET_FILES})