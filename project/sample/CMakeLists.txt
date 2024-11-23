project(sample)

add_executable(sample
    main.cpp
)

target_link_libraries(sample
    PRIVATE
        engine
)

string(REPLACE "/project/sample" "" cgc_dir "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "cgc_dir: ${cgc_dir}")
target_compile_definitions(sample
    PRIVATE
        CGC_DIR="${cgc_dir}"
        MODEL_DIR="${cgc_dir}/model"
        SHADER_DIR="${cgc_dir}/shader"
)
