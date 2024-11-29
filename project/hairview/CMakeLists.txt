project(hairview)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        engine
)

string(REPLACE "/project/hairview" "" cgc_dir "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "cgc_dir: ${cgc_dir}")
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        CGC_DIR="${cgc_dir}"
        MODEL_DIR="${cgc_dir}/model"
        SHADER_DIR="${cgc_dir}/shader"
)
