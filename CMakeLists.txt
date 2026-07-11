cmake_minimum_required(VERSION 3.20)

project(Crawler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(ds_library-Vivek/CollectionsLib-RedisLite)

# -----------------------------
# libcurl
# -----------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED IMPORTED_TARGET libcurl)

add_executable(Crawler
    main.cpp

    src/URLfrontier.cpp
    src/URLvalidator.cpp
    src/URLnormalizer.cpp
    src/fetcher.cpp
    src/frameworkdetector.cpp
    src/renderengine.cpp
    src/KMP.cpp
    src/HTMLparser.cpp
    src/seenstorage.cpp
)

target_include_directories(Crawler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ds_library-Vivek/CollectionsLib-RedisLite/include

    # MSYS2 UCRT64 Headers
    C:/msys64/ucrt64/include
)

target_link_libraries(Crawler PRIVATE
    CollectionsLib
    PkgConfig::LIBCURL

    # Windows Socket Library
    ws2_32
)