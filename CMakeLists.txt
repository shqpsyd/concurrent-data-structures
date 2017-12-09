cmake_minimum_required(VERSION 3.8)
project(cse303_p5)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        bench.cc
        clist.h
        rwlist.h
        chash.h
        support.cc
        support.h
        team.cc
        tree.h)

add_executable(bench bench.cc
        clist.h
        rwlist.h
        chash.h
        shash.h
        support.cc
        support.h
        team.cc
        tree.h)

add_executable(multibench multikeybench.cc
        support.cc
        support.h
        team.cc
        shash2.h)

add_executable(main main.cpp rwlist.h)