cmake_minimum_required(VERSION 3.25)

project(game_of_life
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(SDL3 REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
    src/main.c
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL3::SDL3
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)