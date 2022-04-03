project(ste-testing-example-functions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ste-testing-example-functions main.cpp)

# Don't do that, that's for the sake of example.
target_link_directories(ste-testing-example-functions PRIVATE "../../include/")
