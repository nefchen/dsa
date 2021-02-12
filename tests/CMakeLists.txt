cmake_minimum_required(VERSION 3.16.3)
project(deadly_space_adventures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
find_package(Catch2 REQUIRED)

add_executable(dsa_tests
	signal_tests.cpp
	comm_tests.cpp
)

target_include_directories(dsa_tests PRIVATE ../src)
target_link_libraries(dsa_tests SDL2 SDL2_ttf pthread Catch2::Catch2WithMain)

