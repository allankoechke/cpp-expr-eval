#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdexcept>
#include <string>
#include <format>
#include <unordered_map>

#include "runtime.h"
#include "../frontend/parser.h"

class Interpreter {
public:
    Interpreter() = default;

    RuntimeVar eval(const std::string& input) {
        parser.parse(input);
        return parser.root().eval(m_vars);
    }

    void addVar(const std::string& ident, RuntimeVar val) {
        m_vars[ident] = val;
    }

    RuntimeVar getVar(const std::string& ident) {
        if(!m_vars.contains(ident))
            throw std::runtime_error(std::format("Undeclared variable `{}` not found!", ident));
        return m_vars[ident];
    }

private:
    Parser parser;
    std::unordered_map<std::string, RuntimeVar> m_vars;
};

#endif // INTERPRETER_H
