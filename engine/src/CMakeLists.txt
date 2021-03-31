include_directories(${LGLL_SOURCE_DIR}/lgll/glad/include)
link_directories(${LGLL_SOURCE_DIR}/lgll/glad/src)

include_directories(${LGLL_SOURCE_DIR}/lgll/include)
link_directories(${LGLL_SOURCE_DIR}/lgll/src)

include_directories(${LGLL_SOURCE_DIR}/engine/include)
add_executable(engine main.cpp)

target_link_libraries(engine lgll)
