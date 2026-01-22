#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <utility>
#include <vector>

enum class TokenType {
    TOK_NUMBERS_LIT,
    TOK_STRING_LIT,
    TOK_BOOL_LIT,
    TOK_NULL_LIT,
    TOK_IDENT_LIT,

    TOK_ADD_OP,
    TOK_SUB_OP,
    TOK_DIV_OP,
    TOK_MULT_OP,

    TOK_OPEN_PAREN, // (
    TOK_CLOSE_PAREN, // )

    TOK_EOF
};

struct Token {
    TokenType type;
    std::string name, value;

    Token(TokenType _type, std::string _name, std::string _value);
};

class Lexer {
public:
    Lexer() = default;

    // Given user input, convert it to vector of token object
    const std::vector<Token> &tokenize(const std::string &src);

    // Return the vector of token object
    const std::vector<Token> &tokens();

private:
    void tokenizeNumbers();

    void tokenizeString();

    void tokenizeIdentifiers();

    void tokenizeOperators();

    void skipWhitespaces();

    [[nodiscard]] char peek() const;

    char advance();

    [[nodiscard]] bool eof() const;

    void expect(char c, const std::string &error);


    int m_cursor = 0;
    std::string m_src;
    std::vector<Token> m_tokens;
};

#endif // LEXER_H
