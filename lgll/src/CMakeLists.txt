project(lgll)

include_directories(${LGLL_SOURCE_DIR}/lgll/glad/include)
link_directories(${LGLL_SOURCE_DIR}/lgll/glad/src)

include_directories(${LGLL_SOURCE_DIR}/lgll/include)
add_library(lgll
        app.cpp
        geometry.cpp
        image.cpp
        entity.cpp
        scene.cpp
        shader.cpp)

target_link_libraries(lgll PRIVATE glad)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(GLM REQUIRED IMPORTED_TARGET glm)
pkg_check_modules(SDL2IMAGE REQUIRED IMPORTED_TARGET SDL2_image)

target_link_libraries(lgll PUBLIC
    PkgConfig::GLFW3
    PkgConfig::GLM
    PkgConfig::SDL2IMAGE)

