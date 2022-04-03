project(ste-testing-example-class LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ste-testing-example-class main.cpp some_class_tests.hpp some_class_tests.cpp)

# Don't do that, that's for the sake of example.
target_link_directories(ste-testing-example-class PRIVATE "../../include/")
