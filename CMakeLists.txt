cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SOURCE src/*.cpp)

set(SOURCE_LIB lib/wingetopt.c)

include_directories(lib)

add_library(wingetopt STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE})
target_link_libraries(main wingetopt)
