cmake_minimum_required(VERSION 2.8)
project(clang_demos)

file(GLOB CURDIR RELATIVE ${CMAKE_CURRENT_BINARY_DIR} *.cpp)
message("${CURDIR}")

foreach(ele ${CURDIR})
  string(REPLACE ".cpp" ".out" filename ${ele})
  add_executable(${filename} ${ele})
endforeach(ele ${CURDIR})
