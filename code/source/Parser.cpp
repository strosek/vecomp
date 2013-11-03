/* Definition of member functions of class Parser */

#include "../headers/Parser.hpp"

#include "../headers/Scanner.hpp"

#include <string>
#include <cstdlib>
#ifdef DEBUG
# include <iostream>
#endif

using namespace std;

Parser::Parser() :
  m_errors(0),
  m_warnings(0),
  m_tokenNo(0),
  m_currentToken(),
  m_scanner(),
  m_errorReporter()
{
}

int Parser::getErrors() const {
  return m_errors;
}

int Parser::getWarnings() const {
  return m_warnings;
}

int Parser::parse(const string& fileName) {
  m_scanner.scan(fileName);

  if ((m_errors = m_scanner.getErrors()) == 0) {
    if (program() == EXIT_FAILURE)
      return EXIT_FAILURE;                     

#ifdef DEBUG
    cout << "expected tokens: " << m_tokenNo << ", got: " <<
        m_scanner.getMaxTokens() << endl;
#endif
    if (m_tokenNo != m_scanner.getMaxTokens()) {
      m_errorReporter.writeError(
          "codigo incompleto, cantidad de tokens incongruente");
      ++m_errors;
      return EXIT_FAILURE;
    }
  }
  else
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int Parser::andoperator() {
  return EXIT_SUCCESS;
}

int Parser::assign() {
  return EXIT_SUCCESS;
}

int Parser::block() {
  return EXIT_SUCCESS;
}

int Parser::caseStatement() {
  return EXIT_SUCCESS;
}

int Parser::command() {
  return EXIT_SUCCESS;
}

int Parser::constant() {
  return EXIT_SUCCESS;
}

int Parser::dimension() {
  return EXIT_SUCCESS;
}

int Parser::exponent() {
  return EXIT_SUCCESS;
}

int Parser::expression() {
  return EXIT_SUCCESS;
}

int Parser::forStatement() {
  return EXIT_SUCCESS;
}

int Parser::function() {
  return EXIT_SUCCESS;
}

int Parser::functionSign() {
  return EXIT_SUCCESS;
}

int Parser::ifStatement() {
  return EXIT_SUCCESS;
}

int Parser::import() {
  return EXIT_SUCCESS;
}

int Parser::multiplication() {
  return EXIT_SUCCESS;
}

int Parser::notOperator() {
  return EXIT_SUCCESS;
}

int Parser::parameterList() {
  return EXIT_SUCCESS;
}

int Parser::parameters() {
  return EXIT_SUCCESS;
}

int Parser::print() {
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

int Parser::useDimenison() {
  return EXIT_SUCCESS;
}

int Parser::read() {
  return EXIT_SUCCESS;
}

int Parser::relational() {
  return EXIT_SUCCESS;
}

int Parser::returnExpression() {
  return EXIT_SUCCESS;
}

int Parser::returnType() {
  return EXIT_SUCCESS;
}

int Parser::sign() {
  return EXIT_SUCCESS;
}

int Parser::statement() {
  return EXIT_SUCCESS;
}

int Parser::term() {
  return EXIT_SUCCESS;
}

int Parser::type() {
  return EXIT_SUCCESS;
}

int Parser::useParameters() {
  return EXIT_SUCCESS;
}

int Parser::variables() {
  return EXIT_SUCCESS;
}

bool Parser::isTokenCorrect(TokenType_t expectedToken)
{
  if (m_errors < m_errorsLimit)
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getToken() != expectedToken)
    {
      m_errorReporter->writeSyntaxError(expectedToken,
                                        m_currentToken.getToken());
      ++m_errors;
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
  if (m_errors < m_errorsLimit)
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getLexeme().compare(expectedLexeme) != 0)
    {
      m_errorReporter->writeSyntaxError(expectedLexeme,
                                        m_currentToken.getLexeme());
      ++m_errors;
      return false;
    }
  }
  else
  {
    return false;
  }

  return true;
}

