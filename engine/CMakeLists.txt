cmake_minimum_required(VERSION 3.18)
project(engine)

add_library(engine STATIC
    glad/src/gl.c
    src/util.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/HairLoader.cpp
    src/HairRenderer.cpp
    src/HairModel.cpp
    src/Strand.cpp
)

target_include_directories(engine PUBLIC
    glad/include
    include
)

target_link_libraries(engine
    PUBLIC
        OpenGL::GL
        glfw
        glm::glm
        Eigen3::Eigen
)