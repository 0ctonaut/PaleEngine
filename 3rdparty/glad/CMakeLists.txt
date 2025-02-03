project(glad)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(glad STATIC ${SOURCES} ${HEADERS})
# target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT)
# target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT_BUILD)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)