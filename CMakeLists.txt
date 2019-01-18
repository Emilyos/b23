cmake_minimum_required(VERSION 2.8)
project(b23)

set(CMAKE_CXX_STANDARD)

add_executable(b23 main1.cpp Key.h Value.h LCRS_BalancedTree.cpp LCRS_BalancedTree.h Node.cpp Node.h main1.cpp)