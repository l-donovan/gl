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
