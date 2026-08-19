cmake_minimum_required(VERSION 3.20)
project(MiniRedis VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -Wpedantic)
endif()

# Core data store library
add_library(redis_core src/store.cpp)
target_include_directories(redis_core PUBLIC include)

# TCP network server library
add_library(redis_server src/server.cpp)
target_link_libraries(redis_server PUBLIC redis_core)
target_include_directories(redis_server PUBLIC include)

# Executable entry point
add_executable(mini_redis src/main.cpp)
target_link_libraries(mini_redis PRIVATE redis_server)
