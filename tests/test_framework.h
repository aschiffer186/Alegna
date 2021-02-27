#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <map>
#include <iostream>
#include <string>

#define SET_UP_TESTS()\
    std::map<std::string, std::string> test_statuses;\
    int num_passed = 0;\
    int num_failed = 0;

//Defines the function header for a new test with the sepcified name 
//and test description. Prints the name of the test and a description
//for the test. Creates a try block for the test
//@param test the name of the test
//@param dec a description for the test
#define MAKE_TEST(test, desc)\
    void test_##test(const std::string& test_name = #desc)\
    {\
        std::cout << "Begin test " << test_name << std::endl;\
        std::cout << #desc << std::endl;\
        try\
        {

//Compares the contents of the two containers expected and actual. 
//If they are different, prints an error message with a brief description 
//and terminates the test.
//@param expected the expected values 
//@param actual the actual values
#define CONTENTS_TEST(expected, actual)\
    if (expected.size() != actual.size())\
    {\
        std::cout << "Test " << test_name << " FAILED! Expected size: " << expected.size() << ", Actual size: " << actual.size();\
        test_statuses[test_name] = "FAILED";\
        ++num_failed;\
        return;\
    }\
    for (size_t i = 0; i < expected.size(); ++i)\
    {\
        \
        if(expected[i] != actual[i])\
        {\
            std::string desc = "contents index " + std::to_string(i);\
            std::cout << "Test " << test_name << "FAILED! Expected " << desc << ": " << expected[i] << \
            " Actual " << desc << ": " << actual[i] << std::endl;\
            test_statuses[test_name] = "PASSED";\
            ++num_failed;\
            return;\
        }\
    }\

//Prints a message if the test passes.
#define PASSED()\
    std::cout << "Test " << test_name << " PASSED!" << std::endl;\
    test_statuses[test_name] ="PASSED";\
    ++num_passed;\
    return;

//Concludes a test. Provides a catch block that handles any unexpected exceptions and 
//prints an error message.
#define END_TEST()\
    } catch (...) {\
        std::cout << "Unexpected Exception. Test "<< test_name << " Failed!" << std::endl;\
        test_statuses[test_name] = "FAILED";\
        ++num_failed;\
    }}  

//Prints how many tests passed and failed. Then prints which tests passed 
//and which tests failed.
#define TEST_SUMMARY() \
    std::cout << "Test summary" << std::endl;\
    std::cout << num_passed << " tests passed" << std::endl;\
    std::cout << num_failed << " tests failed" << std::endl;\
    std::cout << "\nPrinting status of individual test" << std::endl;\
    for(const auto& test: test_statuses)\
    {\
        std::cout << test.first << ": " << test.second << std::endl;\
    }

#endif