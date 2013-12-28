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
  void andOperation();
  void argumentsList();
  void assign();
  void block();
  void caseStatement();
  void command();
  void constant();
  void constantsDeclaration();
  void dimension();
  void exponent();
  void expression();
  void forStatement();
  void functionCall();
  void functionDeclaration();
  void ifStatement();
  void import();
  void multiplication();
  void notOperation();
  void parameterList();
  void print();
  void program();
  void read();
  void relationalOperation();
  void returnExpression();
  void returnType();
  void sign();
  void statements();
  void sumOperation();
  void term();
  void variablesDeclaration();
  void variablesList();

  void advanceToken();
  void checkToken(TokenType_t token);
  void checkLexeme(const std::string& lexeme);
  void checkNativeDataType();
  void checkLiteral();
  void ignoreNewLines();
  bool isNativeDataType(const std::string& lexeme);
  bool isLiteral(TokenType_t token);

  TokenLexeme    m_currentToken;
  Scanner        m_scanner;
  ErrorReporter* m_errorReporter;
  int            m_maxErrors;
  int            m_nTokensProcessed;
};

#endif /* PARSER_HPP */

