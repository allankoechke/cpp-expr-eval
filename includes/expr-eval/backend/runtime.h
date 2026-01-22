#ifndef RUNTIME_H
#define RUNTIME_H

#include <complex>
#include <stdexcept>
#include <string>
#include <format>

struct RuntimeVar {
    enum class RuntimeVarType{
        STRING,
        NUMBER,
        NIL,
        BOOL
    } type;
    std::string value;
    bool b_value = false;
    double d_value = 0.0;

    RuntimeVar()
        : type(RuntimeVarType::NIL),
        value("nil") {}

    explicit RuntimeVar(const std::string& v)
        : type(RuntimeVarType::STRING),
        value(v) {}

    explicit RuntimeVar(const bool t)
        : type(RuntimeVarType::BOOL),
        b_value(t),
        value(t ? "true" : "false") {}

    explicit RuntimeVar(const double d)
        : type(RuntimeVarType::NUMBER),
        d_value(d),
        value(std::format("{}", d)) {}

    std::string typeStr() {
        switch(type) {
            case RuntimeVarType::STRING: return "STRING";
            case RuntimeVarType::NUMBER: return "NUMBER";
            case RuntimeVarType::NIL: return "NIL";
            case RuntimeVarType::BOOL: return "BOOL";
            default: return "?";
        }
    }

    double toDouble() {
        if(type == RuntimeVarType::NUMBER)
            return stod(value);

        throw std::runtime_error("Expected number type");
    }

    std::string toString() {
        return value;
    }

    bool toBool() {
        if(value=="false" || value == "nil") return false;
        if(type == RuntimeVarType::NUMBER) return toDouble() != 0.0f;
        return true;
    }

    RuntimeVar operator+(RuntimeVar& other) {
        if(type != other.type)
            throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                typeStr(), other.typeStr()));

        if(type == RuntimeVarType::STRING) {
            return RuntimeVar{ value + other.value };
        }

        if(type == RuntimeVarType::NUMBER) {
            auto res = toDouble() + other.toDouble();
            return RuntimeVar{ res };
        }

        throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
    }

    RuntimeVar operator-(RuntimeVar& other) {
        if(type != other.type)
            throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                typeStr(), other.typeStr()));

        if(type == RuntimeVarType::NUMBER) {
            auto res = toDouble() - other.toDouble();
            return RuntimeVar{ res };
        }

        throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
    }

    RuntimeVar operator*(RuntimeVar& other) {
        if(type != other.type)
            throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                typeStr(), other.typeStr()));

        if(type == RuntimeVarType::NUMBER) {
            auto res = toDouble() * other.toDouble();
            return RuntimeVar{ res };
        }

        throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
    }

    RuntimeVar operator/(RuntimeVar& other) {
        if(type != other.type)
            throw std::runtime_error(std::format("Expected same types to op '+' but found {} and {}.",
                typeStr(), other.typeStr()));

        if(type == RuntimeVarType::NUMBER) {
            auto res = toDouble() / other.toDouble();
            return RuntimeVar{ res };
        }

        throw std::runtime_error(std::format("Op '+' not supported for type {}", typeStr()));
    }
};

#endif // RUNTIME_H
