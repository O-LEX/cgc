project(cuda LANGUAGES CXX CUDA)

if(NOT CMAKE_CUDA_COMPILER)
    message(FATAL_ERROR "CUDA compiler not found. Please ensure CUDA is properly installed and configured.")
endif()

execute_process(COMMAND ${CMAKE_CUDA_COMPILER} --version
                OUTPUT_VARIABLE CUDA_VERSION
                ERROR_VARIABLE CUDA_ERROR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT CUDA_VERSION)
    message(FATAL_ERROR "Failed to determine CUDA version: ${CUDA_ERROR}")
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
