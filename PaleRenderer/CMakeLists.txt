project(PaleRenderer)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(PaleRenderer ${SOURCES} ${HEADERS})
# add_library(PaleRenderer SHARED ${SOURCES} ${HEADERS})

target_include_directories(PaleRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


# --- 3dparty ---
#target_link_libraries(PaleRenderer PRIVATE GLFW)
target_include_directories(PaleRenderer PRIVATE ${CMAKE_SOURCE_DIR}/3dparty/GLFW/include)
target_link_libraries(PaleRenderer PRIVATE ${CMAKE_SOURCE_DIR}/3dparty/GLFW/lib/glfw3.lib)
target_link_libraries(PaleRenderer PRIVATE glad)

# --- macros ---
target_compile_definitions(PaleRenderer PRIVATE DLL_EXPORT)