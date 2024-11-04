cmake_minimum_required(VERSION 3.10)
project(cgc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Eigen3 3.4 REQUIRED NO_MODULE)

set(SOURCES
    main.cpp
    external/glad/src/gl.c
    src/util.cpp
    src/Camera.cpp
    src/Node.cpp
    src/Rod.cpp
    src/Shader.cpp
    src/Camera.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    external/glad/include
    include
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${PROJECT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    glm::glm
    Eigen3::Eigen
)
