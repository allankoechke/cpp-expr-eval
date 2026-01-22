#include <exception>
#include <iostream>
#include <string>
#include <format>

#include "../include/expr-eval/frontend/lexer.h"
#include "../include/expr-eval/frontend/parser.h"
#include "../include/expr-eval/frontend/ast.h"
#include "../include/expr-eval/backend/runtime.h"
#include "../include/expr-eval/backend/interpreter.h"

// REPL
int main() {
    // Create an Interpreter instance
    Interpreter ip;

    ip.addVar("f_name", RuntimeVar(std::string{"John"}));
    ip.addVar("l_name", RuntimeVar(std::string{"Doe"}));
    ip.addVar("x", RuntimeVar(23.45));
    ip.addVar("PI", RuntimeVar(3.14));

    while(true) {
        std::cout << ">>> " << std::flush;

        std::string input;
        std::getline(std::cin, input);

        // Exit REPL using `exit` command
        if(input == "exit") break;

       try {
            auto res = ip.eval(input);
            std::cout << res.toString() << std::endl;
       } catch(const std::exception& e) {
           std::cout << "error: " << e.what() << std::endl;
       }
    }
    return 0;
}
