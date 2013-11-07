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

  int parse();

  Parser& operator=(const Parser& rhs);
private:
  int andoperator(bool isLookedForward);
  int assign(bool isLookedForward);
  int block(bool isLookedForward);
  int caseStatement(bool isLookedForward);
  int command(bool isLookedForward);
  int constantDeclaration(bool isLookedForward);
  int dimension(bool isLookedForward);
  int exponent(bool isLookedForward);
  int expression(bool isLookedForward);
  int forStatement(bool isLookedForward);
  int functionDeclaration(bool isLookedForward);
  int functionSign(bool isLookedForward);
  int ifStatement(bool isLookedForward);
  int import(bool isLookedForward);
  int multiplication(bool isLookedForward);
  int notOperator(bool isLookedForward);
  int parameterList(bool isLookedForward);
  int parameters(bool isLookedForward);
  int print(bool isLookedForward);
  int program();
  int useDimenison(bool isLookedForward);
  int read(bool isLookedForward);
  int relational(bool isLookedForward);
  int returnExpression(bool isLookedForward);
  int returnType(bool isLookedForward);
  int sign(bool isLookedForward);
  int statement(bool isLookedForward);
  int term(bool isLookedForward);
  int type(bool isLookedForward);
  int useParameters(bool isLookedForward);
  int variables(bool isLookedForward);

  bool isLexemeCorrect(const std::string& lexeme);
  bool isTokenCorrect(TokenType_t token);

  int            m_tokenNo;
  TokenLexeme    m_currentToken;
  Scanner        m_scanner;
  ErrorReporter* m_errorReporter;
};

#endif /* PARSER_HPP */

