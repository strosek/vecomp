/* Declaration of class Parser */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>
#include <utility>

#include "CodeGenerator.hpp"
#include "ErrorReporter.hpp"
#include "Scanner.hpp"
#include "SemanticChecker.hpp"
#include "TokenLexeme.hpp"

class Parser
{
public:
  Parser();
  Parser(const Parser& source);

  void parse();

  void setErrorReporter(ErrorReporter* errorReporter);
  void setScanner(Scanner* scanner);
  void setOutputType(OutputType_t type);

  Parser& operator=(const Parser& rhs);
private:
  void andOperation();
  void argumentsList();
  void assign();
  void block();
  void switchStatement();
  void command();
  void constant();
  void constantsDeclaration();
  void dimension();
  void dimensionUse();
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

  bool isLiteral(TokenType_t token);
  bool isNativeDataType(const std::string& lexeme);
  void advanceToken();
  void checkLexeme(const std::string& lexeme);
  void checkLiteral();
  void checkNativeDataType();
  void checkToken(TokenType_t token);
  void ignoreNewLines();
  void resolveVariablesType(NativeType_t type);

  TokenLexeme            m_currentToken;
  Scanner*               m_scanner;
  ErrorReporter*         m_errorReporter;
  CodeGenerator          m_codeGenerator;
  unsigned int           m_maxErrors;
  unsigned int           m_maxRuleIterations;
  size_t                 m_nTokensProcessed;
  SemanticChecker        m_semanticChecker;
  std::stack<SymbolData> m_functions;
  std::queue<SymbolData> m_variables;
};

#endif /* PARSER_HPP */

