cmake_minimum_required(VERSION 3.18)
project(cuda LANGUAGES CXX CUDA)

find_package(CUDA REQUIRED)

if(NOT CMAKE_CUDA_COMPILER)
    message(FATAL_ERROR "CUDA compiler not found. Please ensure CUDA is properly installed and configured.")
endif()

message(STATUS "Using CUDA compiler: ${CMAKE_CUDA_COMPILER}")
message(STATUS "CUDA version: ${CUDA_VERSION}")

add_executable(${PROJECT_NAME}
    main.cpp
    pbd.cu
)

if(DEFINED ENV{CUDA_ARCH})
  set(CUDA_ARCHITECTURES "$ENV{CUDA_ARCH}")
  message(STATUS "Using CUDA architectures from environment variable: ${CUDA_ARCHITECTURES}")
else()
  set(CUDA_ARCHITECTURES "75")
  message(STATUS "Using default CUDA architectures: ${CUDA_ARCHITECTURES}")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    CUDA_STANDARD 14
    CUDA_ARCHITECTURES ${CUDA_ARCHITECTURES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CUDA_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} 
  PRIVATE 
    ${CUDA_LIBRARIES}
    engine
)

string(REPLACE "/project/cuda" "" cgc_dir "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "cgc_dir: ${cgc_dir}")
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        CGC_DIR="${cgc_dir}"
        MODEL_DIR="${cgc_dir}/model"
        SHADER_DIR="${cgc_dir}/shader"
)