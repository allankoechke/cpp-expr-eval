#include "../../include/expr-eval/frontend/ast.h"

#include <stdexcept>

Node::Node(const NodeType t, std::string name)
    : name(std::move(name)),
      type(t) {
}

Node::~Node() = default;

picojson::value Node::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    return picojson::value(obj);
}

RuntimeVar Node::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    return {};
}

Program::Program()
    : Node(NodeType::PROGRAM, "Program") {
}

void Program::addNode(std::unique_ptr<Node> node) {
    m_ast.push_back(std::move(node));
}

void Program::clear() {
    m_ast.clear();
}

picojson::value Program::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);

    picojson::array arr;
    for (const auto &e: m_ast) {
        arr.push_back(e->dump());
    }

    obj["body"] = picojson::value(arr);
    return picojson::value(obj);
}

RuntimeVar Program::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    RuntimeVar res;

    for (const auto &node: m_ast) {
        res = node->eval(vars);
    }

    return res;
}

Expr::Expr(const NodeType t, const std::string &name) : Node{t, name} {
}

BinaryExpr::BinaryExpr(std::unique_ptr<Node> left,
                       std::string op, std::unique_ptr<Node> right)
    : Expr(NodeType::BINARY_EXPR, "BinaryExpr"),
      left(std::move(left)),
      right(std::move(right)),
      op(std::move(op)) {
}

picojson::value BinaryExpr::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    obj["left"] = left->dump();
    obj["op"] = picojson::value(op);
    obj["right"] = right->dump();
    return picojson::value(obj);
}

RuntimeVar BinaryExpr::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    auto _l = left->eval(vars);
    auto _r = right->eval(vars);

    if (op == "+") return _l + _r;
    if (op == "-") return _l - _r;
    if (op == "*") return _l * _r;
    if (op == "/") return _l / _r;

    throw std::runtime_error(std::format("Unimplemented op `{}`", op));
}

NumberLiteral::NumberLiteral(const std::string &value)
    : Expr(NodeType::NUMBER_LIT, "NumberLiteral"),
      value(value) {
    d = stod(value);
}

picojson::value NumberLiteral::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    obj["value"] = picojson::value(d);
    return picojson::value(obj);
}

RuntimeVar NumberLiteral::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    return RuntimeVar{d};
}

BooleanLiteral::BooleanLiteral(const std::string &value)
    : Expr(NodeType::BOOLEAN_LIT, "BooleanLiteral"),
      value(value), b_value(value == "true") {
}

picojson::value BooleanLiteral::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    obj["value"] = picojson::value(b_value);
    return picojson::value(obj);
}

RuntimeVar BooleanLiteral::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    return RuntimeVar{b_value};
}

StringLiteral::StringLiteral(std::string value)
    : Expr(NodeType::STRING_LIT, "StringLiteral"),
      value(std::move(value)) {
}

picojson::value StringLiteral::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    obj["value"] = picojson::value(value);
    return picojson::value(obj);
}

RuntimeVar StringLiteral::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    return RuntimeVar{value};
}

IdentifierLiteral::IdentifierLiteral(std::string value)
    : Expr(NodeType::IDENT_LIT, "IdentifierLiteral"),
      value(std::move(value)) {
}

picojson::value IdentifierLiteral::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    obj["value"] = picojson::value(value);
    return picojson::value(obj);
}

RuntimeVar IdentifierLiteral::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    if (!vars.contains(value)) {
        throw std::runtime_error(std::format("Use of undefined variable `{}`", value));
    }

    return vars[value];
}

NullLiteral::NullLiteral()
    : Expr(NodeType::NIL_LIT, "NullLiteral") {
}

picojson::value NullLiteral::dump() {
    picojson::object obj;
    obj["name"] = picojson::value(name);
    obj["value"] = picojson::value(value);
    return picojson::value(obj);
}

RuntimeVar NullLiteral::eval(std::unordered_map<std::string, RuntimeVar> &vars) {
    return RuntimeVar{};
}
