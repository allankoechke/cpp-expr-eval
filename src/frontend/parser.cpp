#include "../../include/expr-eval/frontend/parser.h"
#include "../../include/expr-eval/frontend/lexer.h"
#include "../../include/expr-eval/frontend/ast.h"

#include <exception>
#include <memory>
#include <stdexcept>
#include <format>

void Parser::parse(const std::string &src) {
    lexer.tokenize(src);
    m_cursor = 0;
    m_program.clear();

    while (peek().type != TokenType::TOK_EOF) {
        m_program.addNode(parseExpr());
    }
}

picojson::value Parser::dump() {
    return m_program.dump();
}

Program &Parser::root() {
    return m_program;
}

std::unique_ptr<Node> Parser::parseExpr() {
    return parseOr();
}

// a || b
std::unique_ptr<Node> Parser::parseOr() {
    auto left = parseAnd();
    while (!eof() && (
               peek().type == TokenType::TOK_OR
           )) {
        std::string op = advance().value;
        auto right = parseAnd();

        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Node> Parser::parseAnd() {
    auto left = parseEquality();
    while (!eof() && ( peek().type == TokenType::TOK_AND )) {
        std::string op = advance().value;
        auto right = parseEquality();

        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Node> Parser::parseEquality() {
    auto left = parseRelational();
    while (!eof() && ( peek().type == TokenType::TOK_EQ || peek().type == TokenType::TOK_NEQ )) {
        std::string op = advance().value;
        auto right = parseRelational();

        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Node> Parser::parseRelational() {
    auto left = parseAdditives();
    while (!eof() && ( peek().type == TokenType::TOK_LT
        || peek().type == TokenType::TOK_LT_EQ
        || peek().type == TokenType::TOK_GT
        || peek().type == TokenType::TOK_GT_EQ )) {
        std::string op = advance().value;
        auto right = parseAdditives();

        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Node> Parser::parseAdditives() {
    auto left = parseFactors();
    while (!eof() && (
               peek().type == TokenType::TOK_ADD_OP ||
               peek().type == TokenType::TOK_SUB_OP
           )) {
        std::string op = advance().value;
        auto right = parseFactors();

        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Node> Parser::parseFactors() {
    auto left = parsePrimary();
    while (!eof() && (
               peek().type == TokenType::TOK_MULT_OP ||
               peek().type == TokenType::TOK_DIV_OP ||
               peek().type == TokenType::TOK_MOD_OP
           )) {
        std::string op = advance().value;
        auto right = parsePrimary();

        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<Node> Parser::parsePrimary() {
    switch (peek().type) {
        case TokenType::TOK_NUMBERS_LIT: {
            auto token = advance();
            return std::make_unique<NumberLiteral>(token.value);
        }
        case TokenType::TOK_STRING_LIT: {
            auto token = advance();
            return std::make_unique<StringLiteral>(token.value);
        }
        case TokenType::TOK_BOOL_LIT: {
            auto token = advance();
            return std::make_unique<BooleanLiteral>(token.value);
        }
        case TokenType::TOK_NULL_LIT: {
            auto token = advance();
            return std::make_unique<NullLiteral>();
        }
        case TokenType::TOK_IDENT_LIT: {
            auto token = advance();
            return std::make_unique<IdentifierLiteral>(token.value);
        }
        case TokenType::TOK_OPEN_PAREN: {
            advance();
            auto left = parseExpr();
            expect(TokenType::TOK_CLOSE_PAREN,
                   "Expected closing parens ')' but was not found!");
            return left;
        }
        default: {
            throw std::runtime_error(std::format("Unknown Token `{}`", peek().value));
        }
    }
}

Token Parser::peek() {
    return lexer.tokens().at(m_cursor);
}

Token Parser::advance() {
    return lexer.tokens().at(m_cursor++);
}

bool Parser::eof() {
    return m_cursor >= lexer.tokens().size();
}

void Parser::expect(const TokenType t, const std::string &error) {
    if (!eof() && peek().type == t) {
        advance();
        return;
    }

    throw std::runtime_error(error);
}
