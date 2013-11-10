/* Declaration of class Parser */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

#include "Scanner.hpp"
#include "TokenLexeme.hpp"
#include "ErrorReporter.hpp"
#include "FileReader.hpp"

class Parser
{
public:
  Parser(FileReader* fileReader, ErrorReporter* errorReporter);
  Parser(const Parser& source);

  void parse();

  Parser& operator=(const Parser& rhs);
private:
  void andoperator();
  void assign();
  void block();
  void caseStatement();
  void command();
  void constantDeclaration();
  void dimension();
  void exponent();
  void expression();
  void forStatement();
  void functionDeclaration();
  void functionSign();
  void ifStatement();
  void import();
  void multiplication();
  void notOperator();
  void parameterList();
  void parameters();
  void print();
  void program();
  void useDimenison();
  void read();
  void relational();
  void returnExpression();
  void returnType();
  void sign();
  void statement();
  void term();
  void type();
  void useParameters();
  void variables();

  void checkToken(TokenType_t token, bool isLookedForward);
  void checkLexeme(const std::string& lexeme, bool isLookedForward);

  TokenLexeme    m_currentToken;
  Scanner        m_scanner;
  ErrorReporter* m_errorReporter;
  int            m_maxErrors;
};

#endif /* PARSER_HPP */

