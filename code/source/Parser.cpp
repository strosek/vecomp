/* Definition of member functions of class Parser */

#include "../headers/Parser.hpp"

#include <string>
#include <cstdlib>
#ifdef DEBUG
# include <iostream>
#endif

using namespace std;

Parser::Parser(FileReader* fileReader, ErrorReporter* errorReporter) :
  m_tokenNo(0),
  m_currentToken(),
  m_scanner(Scanner(fileReader, errorReporter)),
  m_errorReporter()
{
}

int Parser::parse() {
  m_scanner.scan();

  if (m_errorReporter->getErrors() < m_errorReporter->getMaxErrors()) {
    if (program() == EXIT_FAILURE)
      return EXIT_FAILURE;                     

#ifdef DEBUG
    cout << "expected tokens: " << m_tokenNo << ", got: " <<
        m_scanner.getMaxTokens() << endl;
#endif
    if (m_tokenNo != m_scanner.getMaxTokens()) {
      m_errorReporter->writeError(
          "codigo incompleto, cantidad de tokens incongruente");
      return EXIT_FAILURE;
    }
  }
  else
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int Parser::andoperator(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::assign(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::block(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::caseStatement(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::command(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::constantDeclaration(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::dimension(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::exponent(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::expression(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::forStatement(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::functionDeclaration(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::functionSign(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::ifStatement(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::import(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::multiplication(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::notOperator(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::parameterList(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::parameters(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::print(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::program() {
  if (!isLexemeCorrect("paquete"))
    return EXIT_FAILURE;

  if (!isLexemeCorrect("principal"))
    return EXIT_FAILURE;

  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_tokenNo;
  if (m_currentToken.getLexeme().compare("var") == 0)
  {
    if (variables(true) == EXIT_FAILURE)
      return EXIT_FAILURE;
  }
  else if (m_currentToken.getLexeme().compare("const") == 0)
  {
    if (constantDeclaration(true) == EXIT_FAILURE)
      return EXIT_FAILURE;
  }
  else if (m_currentToken.getLexeme().compare("funcion") == 0)
  {
    if (functionDeclaration(true) == EXIT_FAILURE)
      return EXIT_FAILURE;
  }
  else
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int Parser::useDimenison(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::read(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::relational(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::returnExpression(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::returnType(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::sign(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::statement(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::term(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::type(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::useParameters(bool isLookedForward) {
  return EXIT_SUCCESS;
}

int Parser::variables(bool isLookedForward) {
  return EXIT_SUCCESS;
}

bool Parser::isTokenCorrect(TokenType_t expectedToken)
{
  if (m_errorReporter->getErrors() < m_errorReporter->getMaxErrors())
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getToken() != expectedToken)
    {
      m_errorReporter->writeSyntaxError(expectedToken,
          m_currentToken.getToken(), m_currentToken.getLine(),
          m_currentToken.getRow());
      return false;
    }
  }
  else
  {
    return false;
  }

  return true;
}

bool Parser::isLexemeCorrect(const string& expectedLexeme)
{
  if (m_errorReporter->getErrors() < m_errorReporter->getMaxErrors())
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getLexeme().compare(expectedLexeme) != 0)
    {
      m_errorReporter->writeSyntaxError(expectedLexeme,
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
      return false;
    }
  }
  else
  {
    return false;
  }

  return true;
}

