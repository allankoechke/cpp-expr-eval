#include "../../include/expr-eval/backend/runtime.h"
#include <stdexcept>

RuntimeVar::RuntimeVar()
    : type(RuntimeVarType::NIL),
      value("nil") {
}

RuntimeVar::RuntimeVar(std::string v)
    : type(RuntimeVarType::STRING),
      value(std::move(v)) {
}

RuntimeVar::RuntimeVar(const bool t)
    : type(RuntimeVarType::BOOL),
      value(t ? "true" : "false"),
      b_value(t) {
}

RuntimeVar::RuntimeVar(const double d)
    : type(RuntimeVarType::NUMBER),
      value(std::format("{}", d)),
      d_value(d) {
}

std::string RuntimeVar::typeStr() const {
    switch (type) {
        case RuntimeVarType::STRING:
            return "string";
        case RuntimeVarType::NUMBER:
            return "number";
        case RuntimeVarType::NIL:
            return "nil";
        case RuntimeVarType::BOOL:
            return "bool";
        default: return "undefined";
    }
}

double RuntimeVar::toDouble() const {
    if (type == RuntimeVarType::NUMBER)
        return stod(value);

    throw std::runtime_error("Expected number type");
}

std::string RuntimeVar::toString() const {
    return value;
}

bool RuntimeVar::toBool() const {
    if (value == "false" || value == "nil") return false;
    if (type == RuntimeVarType::NUMBER) return toDouble() != 0.0f;
    return true;
}

RuntimeVar RuntimeVar::operator+(const RuntimeVar &other) const {
    if (type != other.type)
        throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                                             typeStr(), other.typeStr()));

    if (type == RuntimeVarType::STRING) {
        return RuntimeVar{value + other.value};
    }

    if (type == RuntimeVarType::NUMBER) {
        const auto res = toDouble() + other.toDouble();
        return RuntimeVar{res};
    }

    throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
}

RuntimeVar RuntimeVar::operator-(const RuntimeVar &other) const {
    if (type != other.type)
        throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                                             typeStr(), other.typeStr()));

    if (type == RuntimeVarType::NUMBER) {
        const auto res = toDouble() - other.toDouble();
        return RuntimeVar{res};
    }

    throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
}

RuntimeVar RuntimeVar::operator*(const RuntimeVar &other) const {
    if (type != other.type)
        throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                                             typeStr(), other.typeStr()));

    if (type == RuntimeVarType::NUMBER) {
        const auto res = toDouble() * other.toDouble();
        return RuntimeVar{res};
    }

    throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
}

RuntimeVar RuntimeVar::operator/(const RuntimeVar &other) const {
    if (type != other.type)
        throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                                             typeStr(), other.typeStr()));

    if (type == RuntimeVarType::NUMBER) {
        const auto res = toDouble() / other.toDouble();
        return RuntimeVar{res};
    }

    throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
}
