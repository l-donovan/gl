cmake_minimum_required(VERSION 3.19)

add_compile_definitions(_ROOT_DIR="${CMAKE_SOURCE_DIR}")

project(LGLL)
add_subdirectory(engine)
add_subdirectory(lgll)

set_property(TARGET engine PROPERTY CXX_STANDARD 20)
set_property(TARGET lgll PROPERTY CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

target_link_libraries(lgll PUBLIC glfw)
target_link_libraries(lgll PUBLIC SDL2::SDL2_image)
