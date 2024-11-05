cmake_minimum_required(VERSION 3.10)

project(Acalc VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(acalc
  src/acalc/main.cpp
  src/acalc/shell/shell.cpp
)

target_include_directories(acalc PRIVATE
  ${PROJECT_SOURCE_DIR}/src/acalc/shell
)
