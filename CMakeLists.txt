cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

include_directories(include)

add_executable(game
    src/main.cpp
    src/glad.c
    src/shader.cpp
    src/camera.cpp
    src/render.cpp
    src/input.cpp
    src/object.cpp
    src/field.cpp
)

target_include_directories(game PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(game PRIVATE ${GLFW_LIBRARIES} GL)
