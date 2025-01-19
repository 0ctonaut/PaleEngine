project(GLFW)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(GLFW STATIC IMPORTED)

set_property(TARGET GLFW PROPERTY
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
)

target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)