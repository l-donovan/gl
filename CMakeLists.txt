cmake_minimum_required(VERSION 3.11)

project(gl)

add_executable(gl main.cpp)
target_sources(gl PRIVATE utils/image.cpp utils/app.cpp utils/scene.cpp utils/mesh.cpp utils/geometry.cpp utils/shader.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(GLM REQUIRED IMPORTED_TARGET glm)
pkg_check_modules(SDL2IMAGE REQUIRED IMPORTED_TARGET SDL2_image)

add_subdirectory(glad)
target_link_libraries(gl PRIVATE glad)

target_link_libraries(gl PUBLIC
    PkgConfig::GLFW3
    PkgConfig::GLM
    PkgConfig::SDL2IMAGE)

set_property(TARGET gl PROPERTY CXX_STANDARD 20)
