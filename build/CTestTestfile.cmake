# CMake generated Testfile for 
# Source directory: D:/projects/Crawler
# Build directory: D:/projects/Crawler/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("D:/projects/Crawler/build/URLValidatorTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/URLNormalizerTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/URLFrontierTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/SeenStorageTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/KMPTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/HTMLParserTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/FrameworkDetectorTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/PageStorageTests_e3b0c442_include.cmake")
include("D:/projects/Crawler/build/FetcherTests_e3b0c442_include.cmake")
add_test("RenderEngineTest" "D:/projects/Crawler/build/renderengine_test.exe")
set_tests_properties("RenderEngineTest" PROPERTIES  _BACKTRACE_TRIPLES "D:/projects/Crawler/CMakeLists.txt;244;add_test;D:/projects/Crawler/CMakeLists.txt;0;")
subdirs("ds_library-Vivek/CollectionsLib-RedisLite")
