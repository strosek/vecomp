/* Definition of member functions of class TokenLexeme */

#include "../headers/TokenLexeme.hpp"

#include <string>
#include <map>

using namespace std;

TokenLexeme::TokenLexeme() :
  m_token(TOKEN_IDEN),
  m_lexeme(),
  m_line(1),
  m_row(1)
{
}

TokenLexeme::TokenLexeme(const TokenType_t token, const string& lexeme,
                         unsigned int line, unsigned int row) :
  m_token(token),
  m_lexeme(lexeme),
  m_line(line),
  m_row(row)
{
}

void TokenLexeme::setToken(const TokenType_t token) {
  m_token = token;
}

void TokenLexeme::setLexeme(const string& lexeme) {
  m_lexeme = lexeme;
}

void TokenLexeme::setLine(unsigned int line) {
  m_line = line;
}

void TokenLexeme::setRow(unsigned int row) {
  m_row = row;
}

TokenType_t TokenLexeme::getToken() const {
  return m_token;
}

string TokenLexeme::getLexeme() const {
  return m_lexeme;
}

unsigned int TokenLexeme::getLine() const {
  return m_line;
}

unsigned int TokenLexeme::getRow() const {
  return m_row;
}

string TokenLexeme::getTokenString(TokenType_t token)
{
  static map<TokenType_t, string> tokensMap;
  
  if (tokensMap.size() <= 0)
  {
    tokensMap[TOKEN_INVALID] = "invalido";
    tokensMap[TOKEN_IDEN] = "identificador";
    tokensMap[TOKEN_KEYWORD] = "pal_reservada";
    tokensMap[TOKEN_ARITHOP] = "op_aritmetico";
    tokensMap[TOKEN_LOGICOP] = "op_logico";
    tokensMap[TOKEN_RELOP] = "op_relac";
    tokensMap[TOKEN_DEC] = "entero_dec";
    tokensMap[TOKEN_OCT] = "entero_oct";
    tokensMap[TOKEN_HEX] = "entero_hex";
    tokensMap[TOKEN_FLOAT] = "real";
    tokensMap[TOKEN_STRING] = "cte_alpha";
    tokensMap[TOKEN_LOGICCONST] = "cte_logica";
    tokensMap[TOKEN_DELIMITER] = "delimitador";
    tokensMap[TOKEN_LINECOMMENT] = "comentariolinea";
    tokensMap[TOKEN_MULTICOMMENT] = "multicomentario";
    tokensMap[TOKEN_CHARCONST] = "cte_caracter";
    tokensMap[TOKEN_ASSIGNOP] = "ope_asignacion";
  }

  return tokensMap.at(token);
}

