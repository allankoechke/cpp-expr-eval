#include "../../includes/expr-eval/backend/interpreter.h"
#include <stdexcept>
#include <format>

Interpreter::Interpreter() = default;

RuntimeVar Interpreter::eval(const std::string &input) {
    parser.parse(input);
    return parser.root().eval(m_vars);
}

void Interpreter::addVar(const std::string &ident, RuntimeVar val) {
    m_vars[ident] = std::move(val);
}

RuntimeVar Interpreter::getVar(const std::string &ident) {
    if(!m_vars.contains(ident))
        throw std::runtime_error(std::format("Undeclared variable `{}` not found!", ident));

    return m_vars[ident];
}
