/* Declaration of class TokenLexeme */

#ifndef TOKENLEXEME_HPP
#define TOKENLEXEME_HPP

#include <string>

typedef enum
{
  TOKEN_INVALID = -1,
  TOKEN_IDEN,
  TOKEN_KEYWORD,
  TOKEN_ARITHOP,
  TOKEN_LOGICOP,
  TOKEN_RELOP,
  TOKEN_DEC,
  TOKEN_OCT,
  TOKEN_HEX,
  TOKEN_FLOAT,
  TOKEN_STRING,
  TOKEN_LOGICCONST,
  TOKEN_DELIMITER,
  TOKEN_LINECOMMENT,
  TOKEN_MULTICOMMENT,
  TOKEN_CHARCONST,
  TOKEN_ASSIGNOP,
  TOKEN_NEWLINE
} TokenType_t;


class TokenLexeme
{
public:
  TokenLexeme();
  TokenLexeme(const TokenType_t token, const std::string& lexeme, 
              unsigned int line, unsigned int row);
  
  void setToken(const TokenType_t token);
  void setLexeme(const std::string& lexeme);
  void setLine(unsigned int line);
  void setRow(unsigned int row);
  
  TokenType_t  getToken() const;
  std::string  getLexeme() const;
  unsigned int getLine() const;
  unsigned int getRow() const;

  static std::string  getTokenString(TokenType_t token);
private:
  TokenType_t  m_token;
  std::string  m_lexeme;
  unsigned int m_line;
  unsigned int m_row;
};

#endif // TOKENLEXEME_HPP

