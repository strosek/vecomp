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
  void andOperation(bool isLookedForward);
  void argumentsList(bool isLookedForward);
  void assign(bool isLookedForward);
  void block(bool isLookedForward);
  void caseStatement(bool isLookedForward);
  void command(bool isLookedForward);
  void constant(bool isLookedForward);
  void constantsDeclaration(bool isLookedForward);
  void dimension(bool isLookedForward);
  void exponent(bool isLookedForward);
  void expression(bool isLookedForward);
  void forStatement(bool isLookedForward);
  void functionCall(bool isLookedForward);
  void functionDeclaration(bool isLookedForward);
  void ifStatement(bool isLookedForward);
  void import(bool isLookedForward);
  void multiplication(bool isLookedForward);
  void notOperation(bool isLookedForward);
  void parameterList(bool isLookedForward);
  void print(bool isLookedForward);
  void program();
  void read(bool isLookedForward);
  void relationalOperation(bool isLookedForward);
  void returnExpression(bool isLookedForward);
  void returnType(bool isLookedForward);
  void sign(bool isLookedForward);
  void statements(bool isLookedForward);
  void sumOperation(bool isLookedForward);
  void term(bool isLookedForward);
  void variablesDeclaration(bool isLookedForward);
  void variablesList(bool isLookedForward);

  void advanceToken();
  void checkToken(TokenType_t token, bool isLookedForward);
  void checkLexeme(const std::string& lexeme, bool isLookedForward);
  void checkNativeDataType(bool isLookedForward);
  void checkLiteral(bool isLookedForward);
  void ignoreNewLines(bool isLookedForward);
  bool isNativeDataType(const std::string& lexeme);
  bool isLiteral(TokenType_t token);

  TokenLexeme    m_currentToken;
  Scanner        m_scanner;
  ErrorReporter* m_errorReporter;
  int            m_maxErrors;
};

#endif /* PARSER_HPP */

