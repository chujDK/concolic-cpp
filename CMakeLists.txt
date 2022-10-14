project(concolic-cpp)
cmake_minimum_required(VERSION 3.4)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Z3 REQUIRED z3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS_DEBUG "-DVERBOSE -DDEBUG -ggdb -O0 -g3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})

aux_source_directory(${CMAKE_SOURCE_DIR}/lib ALL_SRC)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(concolic-cpp STATIC ${ALL_SRC})
target_include_directories(concolic-cpp PRIVATE ${Z3_CXX_INCLUDE_DIRS})
target_link_libraries(concolic-cpp PRIVATE ${Z3_LIBRARIES})

target_compile_options(concolic-cpp PRIVATE ${Z3_COMPONENT_CXX_FLAGS})

add_executable(concolic-cpp-test ${CMAKE_SOURCE_DIR}/test/main.cc)
target_link_libraries(concolic-cpp-test PRIVATE concolic-cpp)
