/* Declaration of class Parser */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

#include "Scanner.hpp"
#include "TokenLexeme.hpp"
#include "ErrorReporter.hpp"

class Parser
{
public:
  Parser();

  int getErrors() const;
  int getWarnings() const;
  int parse(const std::string& fileName);
private:
  int andoperator();
  int assign();
  int block();
  int caseStatement();
  int command();
  int constant();
  int dimension();
  int exponent();
  int expression();
  int forStatement();
  int function();
  int functionSign();
  int ifStatement();
  int import();
  int multiplication();
  int notOperator();
  int parameterList();
  int parameters();
  int print();
  int program();
  int useDimenison();
  int read();
  int relational();
  int returnExpression();
  int returnType();
  int sign();
  int statement();
  int term();
  int type();
  int useParameters();
  int variables();

  int           m_errors;
  int           m_warnings;
  int           m_tokenNo;
  TokenLexeme   m_currentToken;
  Scanner       m_scanner;
  ErrorReporter m_errorReporter;
};

#endif /* PARSER_HPP */

