project(PaleEditor)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB PANELS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/Panels/*.h")
file(GLOB PANELS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Panels/*.cpp")

source_group("src" FILES ${HEADERS} ${SOURCES})
source_group("Panels" FILES ${PANELS_HEADERS} ${PANELS_SOURCES})

add_executable(PaleEditor ${SOURCES} ${HEADERS} )

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