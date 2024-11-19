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
    src/Shader.cpp
    src/HairFile.cpp
    src/Strand.cpp
    src/PBD.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    external/glad/include
    include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        glfw
        glm::glm
        Eigen3::Eigen
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${PROJECT_SOURCE_DIR}")
