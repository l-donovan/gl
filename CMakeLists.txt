cmake_minimum_required(VERSION 3.19)

project(LGLL)
add_subdirectory(engine)
add_subdirectory(lgll)

set_property(TARGET engine PROPERTY CXX_STANDARD 20)
set_property(TARGET lgll PROPERTY CXX_STANDARD 20)
