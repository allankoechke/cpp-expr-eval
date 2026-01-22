cmake_minimum_required(VERSION 3.22)
project(expr-eval VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

add_executable(
        expr-eval
        src/main.cpp
        src/frontend/lexer.cpp
        src/frontend/ast.cpp
        src/frontend/parser.cpp
        src/frontend/ast.cpp
)
