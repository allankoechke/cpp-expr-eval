#include "../../includes/expr-eval/frontend/lexer.h"

#include <cctype>
#include <stdexcept>
#include <format>

Token::Token(const TokenType _type, std::string _name, std::string _value)
    : type(_type),
      name(std::move(_name)),
      value(std::move(_value)) {
}

const std::vector<Token> &Lexer::tokenize(const std::string &src) {
    m_src = src; // Assign source string for tokenization
    m_cursor = 0; // Reset cursor position
    m_tokens.clear(); // Reset token list

    while (!eof()) {
        if (std::isdigit(peek()))
            // Tokenize number literals (decimal and floating point)
            tokenizeNumbers();

        else if (peek() == '"')
            // Tokenize string literals like "a string"
            tokenizeString();

        else if (std::isalpha(peek()) || peek() == '_')
            // Parse variables, keywords, functions, etc.
            tokenizeIdentifiers();

        else if (std::isspace(peek()))
            // Skip any whitespaces (tab, space, new line, carriage, etc
            skipWhitespaces();

        else if (peek() == '(')
            m_tokens.emplace_back(
                TokenType::TOK_OPEN_PAREN,
                "TOK_OPEN_PAREN",
                std::string{advance()});
        else if (peek() == ')')
            m_tokens.emplace_back(
                TokenType::TOK_CLOSE_PAREN,
                "TOK_CLOSE_PAREN",
                std::string{advance()});

            // Let's assume at this point, what remains is
            // operators.
        else tokenizeOperators();
    }

    m_tokens.emplace_back(TokenType::TOK_EOF, "TOK_EOF", "");
    return m_tokens;
}

const std::vector<Token> &Lexer::tokens() {
    return m_tokens;
}

void Lexer::tokenizeNumbers() {
    std::string num;

    while (!eof() && std::isdigit(peek())) {
        num += advance();
    }

    m_tokens.emplace_back(TokenType::TOK_NUMBERS_LIT, "TOK_NUMBERS_LIT", num);
}

void Lexer::tokenizeString() {
    std::string str;

    advance();
    while (!eof() && peek() != '"') {
        str += advance();
    }

    expect('"', "Expected '\"' to close the string");
    m_tokens.emplace_back(TokenType::TOK_STRING_LIT, "TOK_STRING_LIT", str);
}

void Lexer::tokenizeIdentifiers() {
    std::string ident;

    while (!eof() && (peek() == '_' || std::isalnum(peek()))) {
        ident += advance();
    }

    if (ident == "true" || ident == "false") {
        m_tokens.emplace_back(TokenType::TOK_BOOL_LIT, "TOK_BOOL_LIT", ident);
    } else if (ident == "nil") {
        m_tokens.emplace_back(TokenType::TOK_NULL_LIT, "TOK_NULL_LIT", ident);
    } else
        m_tokens.emplace_back(TokenType::TOK_IDENT_LIT, "TOK_IDENT_LIT", ident);
}

void Lexer::tokenizeOperators() {
    auto op = advance();

    if (op == '+')
        m_tokens.emplace_back(TokenType::TOK_ADD_OP, "TOK_ADD_LIT", std::string{op});
    else if (op == '-')
        m_tokens.emplace_back(TokenType::TOK_SUB_OP, "TOK_SUB_LIT", std::string{op});
    else if (op == '*')
        m_tokens.emplace_back(TokenType::TOK_MULT_OP, "TOK_MULT_LIT", std::string{op});
    else if (op == '/')
        m_tokens.emplace_back(TokenType::TOK_DIV_OP, "TOK_DIV_LIT", std::string{op});

    else throw std::runtime_error(std::format("Unknown Character `{}` in the input string", peek()));
}

void Lexer::skipWhitespaces() {
    if (std::isspace(peek())) {
        advance();
    }
}

char Lexer::peek() const {
    return m_src[m_cursor];
}

char Lexer::advance() {
    return m_src[m_cursor++];
}

bool Lexer::eof() const {
    return m_cursor >= m_src.size();
}

void Lexer::expect(const char c, const std::string &error) {
    if (!eof() && peek() == c) {
        advance();
        return;
    }

    throw std::runtime_error(error);
}
