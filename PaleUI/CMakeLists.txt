project(PaleUI)

file(GLOB PaleUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(PaleUI ${PaleUI_SOURCES})

target_link_libraries(PaleUI PRIVATE PaleRenderer)

target_include_directories(PaleUI PRIVATE ${CMAKE_SOURCE_DIR}/PaleRenderer/src)
