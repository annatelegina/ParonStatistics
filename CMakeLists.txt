cmake_minimum_required(VERSION 2.8)

set(SOURCE main.cpp )
set(HEADER array.h distance.h searchtree.h strinfile.h table.h wordgroup.h)

set(SOURCE_LIB wingetopt.c wingetopt.h)

add_library(foo STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE} ${HEADER})
