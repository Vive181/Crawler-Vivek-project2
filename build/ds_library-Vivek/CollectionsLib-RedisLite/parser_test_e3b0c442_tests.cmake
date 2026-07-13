add_test([=[ParserTest.ParseSetCommandSetsValue]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/parser_test.exe [==[--gtest_filter=ParserTest.ParseSetCommandSetsValue]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ParserTest.ParseSetCommandSetsValue]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\parsertesting.cpp:13]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[ParserTest.ParseSetCommandOverwritesValue]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/parser_test.exe [==[--gtest_filter=ParserTest.ParseSetCommandOverwritesValue]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ParserTest.ParseSetCommandOverwritesValue]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\parsertesting.cpp:19]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[ParserTest.ParseDelCommandDeletesKey]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/parser_test.exe [==[--gtest_filter=ParserTest.ParseDelCommandDeletesKey]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ParserTest.ParseDelCommandDeletesKey]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\parsertesting.cpp:27]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[ParserTest.ParseIsCaseInsensitiveForCommands]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/parser_test.exe [==[--gtest_filter=ParserTest.ParseIsCaseInsensitiveForCommands]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ParserTest.ParseIsCaseInsensitiveForCommands]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\parsertesting.cpp:35]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[ParserTest.ParseUnknownCommandDoesNotCrash]=]  D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite/parser_test.exe [==[--gtest_filter=ParserTest.ParseUnknownCommandDoesNotCrash]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ParserTest.ParseUnknownCommandDoesNotCrash]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[D:\projects\Crawler\ds_library-Vivek\CollectionsLib-RedisLite\tests\parsertesting.cpp:46]==]
    WORKING_DIRECTORY [==[D:/projects/Crawler/build/ds_library-Vivek/CollectionsLib-RedisLite]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
set(parser_test_TESTS [==[ParserTest.ParseSetCommandSetsValue]==] [==[ParserTest.ParseSetCommandOverwritesValue]==] [==[ParserTest.ParseDelCommandDeletesKey]==] [==[ParserTest.ParseIsCaseInsensitiveForCommands]==] [==[ParserTest.ParseUnknownCommandDoesNotCrash]==])
