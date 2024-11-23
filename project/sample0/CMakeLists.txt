project(sample0)

add_executable(sample0
    main.cpp
)

target_link_libraries(sample0
    PRIVATE
        engine
)

string(REPLACE "/project/sample0" "" cgc_dir "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(sample0
    PRIVATE
        CGC_DIR="${cgc_dir}"
        MODEL_DIR="${cgc_dir}/model"
        SHADER_DIR="${cgc_dir}/shader"
)