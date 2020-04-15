cmake_minimum_required(VERSION 2.8)
project(clang_demos)

add_compile_options(-std=c++11)

file(GLOB CURDIR RELATIVE ${CMAKE_CURRENT_BINARY_DIR} *.cpp)
message("${CURDIR}")

foreach(ele ${CURDIR})
  string(REPLACE ".cpp" ".out" filename ${ele})
  add_executable(${filename} ${ele})
  target_link_libraries(${filename}
    "clang"
    "clangFrontend"
    "clangSerialization"
    "clangBasic"
    "LLVM-9"
    "clangAST"
    "clangParse"
    "clangRewrite"
    )
endforeach(ele ${CURDIR})
