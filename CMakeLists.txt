cmake_minimum_required(VERSION 3.20)

project(clearn LANGUAGES C)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw/)

include_directories(
    lib/glad/include/
    lib/HandmadeMath/
)

file(GLOB SRC_FILES src/main.c src/kp_dynlist.c src/kp_memalloc.c
     src/gfx/models.c)

file(GLOB DEP_FILES lib/glad/src/gl.c)

add_executable(clearn ${SRC_FILES} ${DEP_FILES})

target_link_libraries(clearn glfw)

install(TARGETS clearn RUNTIME DESTINATION bin)
