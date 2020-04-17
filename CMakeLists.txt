cmake_minimum_required(VERSION 2.8)
project(ParonStatistics)

file(GLOB_RECURSE SOURCE src/*.cpp)

set(WIN_SOURCE lib/wingetopt.c)

file(GLOB_RECURSE BLAS_SOURCE lib/liblinear/blas/*.c)
set(LIBLINEAR_SOURCE lib/liblinear/linear.cpp
	lib/liblinear/tron.cpp ${BLAS_SOURCE})

include_directories(lib)

include_directories(/usr/include/python3.6m)
include_directories(/usr/include/python3.6m)


add_library(wingetopt STATIC ${WIN_SOURCE})
add_library(liblinear STATIC ${LIBLINEAR_SOURCE})

add_executable(main ${SOURCE})

target_link_libraries(main wingetopt)
target_link_libraries(main liblinear)

target_link_libraries(main "-L/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions")


