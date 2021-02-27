#Set CMAKE Properties
set(CMAKE_MINIMUM_REQUIRED_VERSION 3.19.0)
project(Compiler)

#Set CXX Prooperties
#This project uses C++ 17 features. 
#Require C++ 17
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

#Add the project source files
file(GLOB src_files src/*.cpp)
set(${PROJECT_NAME}_SOURCE_FILES "")
foreach(file ${src_files})
    set(${PROJECT_NAME}_SOURCE_FILES ${PROJECT_NAME}_SOURCE_FILES ${file})
endforeach()

#Add main executables
add_executable(${PROJRECT_NAME} ${PROJECT_NAME}_SOURCE_FILES)
target_include_directories(include/)

#Build tests
add_executable(Test_Regex_Parser test/test_regex_parser.cpp)
target_include_directories(Test_Regex_Parser PRIVATE tests/)

enable_testing()
add_test(NAME Regex_Parser_Test COMMAND Test_Regex_Parser)