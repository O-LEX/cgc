cmake_minimum_required(VERSION 3.14)
project(cgc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

FetchContent_MakeAvailable(glfw)

set(SOURCES
    main.cpp
    ${PROJECT_SOURCE_DIR}/external/glad/src/gl.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/external/glad/include
)

target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL
    glfw
)
