cmake_minimum_required(VERSION 3.20)

project(Crawler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(ds_library-Vivek/CollectionsLib-RedisLite)


add_executable(Crawler
    main.cpp
    src/URLfrontier.cpp
    src/URLvalidator.cpp
)


target_include_directories(Crawler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ds_library-Vivek/CollectionsLib-RedisLite/include
)

target_link_libraries(Crawler PRIVATE
    CollectionsLib
)