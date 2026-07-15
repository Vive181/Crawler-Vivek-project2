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

# -----------------------------
# Boost
# -----------------------------
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

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
    src/pagestorage.cpp
    src/crawler.cpp
)

target_include_directories(Crawler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ds_library-Vivek/CollectionsLib-RedisLite/include
    C:/msys64/ucrt64/include
)

target_link_directories(Crawler PRIVATE
    C:/msys64/ucrt64/lib
)

target_link_libraries(Crawler PRIVATE
    CollectionsLib
    PkgConfig::LIBCURL
    Boost::system
    Threads::Threads
    ws2_32
    mariadb
)


# =====================================================
# UNIT TESTS - ADD THIS AT THE BOTTOM
# =====================================================

enable_testing()

add_executable(URLValidatorTests
    testing/URLvalidator_testing.cpp
    src/URLvalidator.cpp
)

target_include_directories(URLValidatorTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(URLValidatorTests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(URLValidatorTests)

# URL normalizer

add_executable(URLNormalizerTests
    testing/URLnormalizer_testing.cpp
    src/URLnormalizer.cpp
)

target_include_directories(URLNormalizerTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(URLNormalizerTests PRIVATE
    GTest::gtest_main
)

gtest_discover_tests(URLNormalizerTests)

# URL frontier

add_executable(URLFrontierTests
    testing/URLfrontier_testing.cpp
    src/URLfrontier.cpp
)

target_include_directories(URLFrontierTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ds_library-Vivek/CollectionsLib-RedisLite/include
)

target_link_libraries(URLFrontierTests PRIVATE
    CollectionsLib
    GTest::gtest_main
)

gtest_discover_tests(URLFrontierTests)

# seen storage testing

add_executable(SeenStorageTests
    testing/Seenstorage_testing.cpp
    src/seenstorage.cpp
)

target_include_directories(SeenStorageTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ds_library-Vivek/CollectionsLib-RedisLite/include
)

target_link_libraries(SeenStorageTests PRIVATE
    CollectionsLib
    GTest::gtest_main
)

gtest_discover_tests(SeenStorageTests)

# KMP testing

add_executable(KMPTests
    testing/KMP_testing.cpp
    src/KMP.cpp
)

target_include_directories(KMPTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(KMPTests PRIVATE
    GTest::gtest_main
)

gtest_discover_tests(KMPTests)

# HTML parsing testing

add_executable(HTMLParserTests
    testing/HTMLparser_testing.cpp
    src/HTMLparser.cpp
    src/KMP.cpp
)

target_include_directories(HTMLParserTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(HTMLParserTests PRIVATE
    GTest::gtest_main
)

gtest_discover_tests(HTMLParserTests)

# frameworkdetector testing

add_executable(FrameworkDetectorTests
    testing/FrameworkDetector_testing.cpp
    src/frameworkdetector.cpp
)

target_include_directories(FrameworkDetectorTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(FrameworkDetectorTests PRIVATE
    GTest::gtest_main
)

gtest_discover_tests(FrameworkDetectorTests)

# page storage testing

add_executable(PageStorageTests
    testing/PageStorage_testing.cpp
    src/pagestorage.cpp
)


target_include_directories(PageStorageTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    C:/msys64/ucrt64/include
)


target_link_libraries(PageStorageTests PRIVATE
    GTest::gtest_main
    mariadb
)


gtest_discover_tests(PageStorageTests)

# fetcher testing

add_executable(FetcherTests
    testing/Fetcher_testing.cpp
    src/fetcher.cpp
)

target_include_directories(FetcherTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    C:/msys64/ucrt64/include
)

target_link_libraries(FetcherTests PRIVATE
    GTest::gtest_main
    PkgConfig::LIBCURL
)

gtest_discover_tests(FetcherTests)

# render engine testing

add_executable(renderengine_test
    testing/renderengine_testing.cpp
    src/renderengine.cpp
)

target_link_libraries(renderengine_test
    PRIVATE
    GTest::gtest_main
    curl
    Boost::system
    ws2_32
    mswsock
)

add_test(
    NAME RenderEngineTest
    COMMAND renderengine_test
)