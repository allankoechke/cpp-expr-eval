#ifndef RUNTIME_H
#define RUNTIME_H

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

    RuntimeVar();

    explicit RuntimeVar(std::string  v);

    explicit RuntimeVar(bool t);

    explicit RuntimeVar(double d);

    [[nodiscard]] std::string typeStr() const;

    [[nodiscard]] double toDouble() const;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] bool toBool() const;

    RuntimeVar operator+(const RuntimeVar& other) const;

    RuntimeVar operator-(const RuntimeVar& other) const;

    RuntimeVar operator*(const RuntimeVar& other) const;

    RuntimeVar operator/(const RuntimeVar& other) const;

    RuntimeVar operator%(const RuntimeVar& other) const;

    RuntimeVar operator==(const RuntimeVar& other) const;

    RuntimeVar operator!=(const RuntimeVar& other) const;

    RuntimeVar operator>(const RuntimeVar& other) const;

    RuntimeVar operator>=(const RuntimeVar& other) const;

    RuntimeVar operator<(const RuntimeVar& other) const;

    RuntimeVar operator<=(const RuntimeVar& other) const;
};

#endif // RUNTIME_H
