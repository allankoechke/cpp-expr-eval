#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <unordered_map>

#include "runtime.h"
#include "../frontend/parser.h"

class Interpreter {
public:
    Interpreter();

    RuntimeVar eval(const std::string& input);

    void addVar(const std::string& ident, RuntimeVar val);

    RuntimeVar getVar(const std::string& ident);

private:
    Parser parser;
    std::unordered_map<std::string, RuntimeVar> m_vars;
};

#endif // INTERPRETER_H
