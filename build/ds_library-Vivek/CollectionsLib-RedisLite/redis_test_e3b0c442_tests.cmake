add_test([=[RedisTest.PrimitiveTypes]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/redis_test.exe [==[--gtest_filter=RedisTest.PrimitiveTypes]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[RedisTest.PrimitiveTypes]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\redislitetesting.cpp:34]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[RedisTest.IntegerEdgeCases]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/redis_test.exe [==[--gtest_filter=RedisTest.IntegerEdgeCases]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[RedisTest.IntegerEdgeCases]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\redislitetesting.cpp:49]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[RedisTest.UserDefinedType]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/redis_test.exe [==[--gtest_filter=RedisTest.UserDefinedType]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[RedisTest.UserDefinedType]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\redislitetesting.cpp:60]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[RedisTest.MixedOperations]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/redis_test.exe [==[--gtest_filter=RedisTest.MixedOperations]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[RedisTest.MixedOperations]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\redislitetesting.cpp:75]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
set(redis_test_TESTS [==[RedisTest.PrimitiveTypes]==] [==[RedisTest.IntegerEdgeCases]==] [==[RedisTest.UserDefinedType]==] [==[RedisTest.MixedOperations]==])
