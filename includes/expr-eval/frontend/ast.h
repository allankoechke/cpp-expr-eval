#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "../backend/runtime.h"
#include "../picojson.h"


// ----- NODE TYPE ----- //
enum class NodeType {
    PROGRAM,

    // Primitive types
    STRING_LIT,
    BOOLEAN_LIT,
    NUMBER_LIT,
    NIL_LIT,
    IDENT_LIT,

    // Composites
    BINARY_EXPR,

    // EOF
    END_OF_FILE
};

// ----- NODE ----- //
struct Node {
    std::string name;
    NodeType type;

    Node(NodeType t, std::string name);

    virtual ~Node();

    virtual picojson::value dump();

    virtual RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars);
};


// ----- PROGRAM NODE ----- //
struct Program : Node {
    Program();

    void addNode(std::unique_ptr<Node> node);

    void clear();

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::vector<std::unique_ptr<Node> > m_ast;
};


// ----- EXPR NODE ----- //
struct Expr : Node {
    Expr(NodeType t, const std::string &name);
};


// ----- BINARY EXPR NODE ----- //
struct BinaryExpr : Expr {
    BinaryExpr(std::unique_ptr<Node> left, std::string op, std::unique_ptr<Node> right);

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::unique_ptr<Node> left, right;
    std::string op;
};


// ----- NUMBER LITERAL NODE ----- //
struct NumberLiteral : Expr {
    explicit NumberLiteral(const std::string &value);

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::string value;
    double d;
};


// ----- BOOLEAN LITERAL NODE ----- //
struct BooleanLiteral : Expr {
    explicit BooleanLiteral(const std::string &value);

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::string value;
    bool b_value;
};


// ----- STRING LITERAL NODE ----- //
struct StringLiteral : Expr {
    explicit StringLiteral(std::string value);

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::string value;
};


// ----- IDENTIFIER LITERAL NODE ----- //
struct IdentifierLiteral : Expr {
    explicit IdentifierLiteral(std::string value);

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::string value;
};


// ----- NULL LITERAL NODE ----- //
struct NullLiteral : Expr {
    NullLiteral();

    picojson::value dump() override;

    RuntimeVar eval(std::unordered_map<std::string, RuntimeVar> &vars) override;

private:
    std::string value = "nil";
};


#endif // AST_H
