cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
project(tolima)

set(src src/Point.cpp
        src/Triangle.cpp
        src/TriMesh.cpp
        src/Object.cpp
        src/Shader.cpp
        src/Scene.cpp
        src/Loader.cpp
        src/Matrix.cpp
        src/Clipper.cpp
        src/Camera.cpp
        src/Renderer.cpp
        src/main.cpp)
include_directories(include)

find_package(SDL2 REQUIRED)

add_executable(tolima ${src})

target_link_libraries(tolima SDL2 SDL2main)