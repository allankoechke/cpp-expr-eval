#ifndef PARSER_H
#define PARSER_H

#include <memory>

#include "../picojson.h"
#include "lexer.h"
#include "ast.h"

class Parser {
public:
    Parser() = default;

    void parse(const std::string &src);

    picojson::value dump();

    Program &root();

private:
    std::unique_ptr<Node> parseExpr();

    std::unique_ptr<Node> parseOr(); // ||

    std::unique_ptr<Node> parseAnd(); // &&

    std::unique_ptr<Node> parseEquality(); // == !=

    std::unique_ptr<Node> parseRelational(); // < > <= >=

    std::unique_ptr<Node> parseAdditives();

    std::unique_ptr<Node> parseFactors();

    std::unique_ptr<Node> parsePrimary();

    Token peek();

    Token advance();

    bool eof();

    void expect(TokenType t, const std::string &error);


    int m_cursor = 0;
    Lexer lexer;

    // Store root node for the AST, in our case the Program node
    Program m_program;
};

#endif // PARSER_H
