# TOP

cmake_minimum_required(VERSION 3.0.0)
project(paracpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/conan/deploy/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)

add_subdirectory("lib")

include_directories("include")
add_subdirectory("src")

# include(CTest)
enable_testing()
add_subdirectory("tests")


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
