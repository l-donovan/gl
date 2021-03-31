project(glad)

include_directories(${LGLL_SOURCE_DIR}/lgll/glad/include)
add_library(glad glad.c)
