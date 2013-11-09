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
  m_errorReporter(errorReporter),
  m_maxErrors(5)
{
  m_maxErrors = m_errorReporter->getMaxErrors();
}

void Parser::parse()
{
  m_scanner.scan();

  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  program();

#ifdef DEBUG
    cout << "expected tokens: " << m_tokenNo << ", got: " <<
        m_scanner.getMaxTokens() << endl;
#endif
  if (m_tokenNo != m_scanner.getMaxTokens())
  {
    m_errorReporter->writeError(
        "codigo incompleto, cantidad de tokens incongruente");
  }
}

void Parser::andoperator()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::assign()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::block()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::caseStatement()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::command()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::constantDeclaration()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::dimension()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::exponent()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::expression()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::forStatement()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::functionDeclaration()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::functionSign()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::ifStatement()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::import()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::multiplication()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::notOperator()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::parameterList()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::parameters()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::print()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::program()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("paquete");

  checkLexeme("principal");

  do
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getLexeme().compare("var") == 0)
    {
      variables();
    }
    else if (m_currentToken.getLexeme().compare("const") == 0)
    {
      constantDeclaration();
    }
    else if (m_currentToken.getLexeme().compare("funcion") == 0)
    {
      functionDeclaration();
    }
  } while ((m_currentToken.getLexeme().compare("var")   ||
            m_currentToken.getLexeme().compare("const") ||
            m_currentToken.getLexeme().compare("funcion")) &&
            m_tokenNo < m_scanner.getMaxTokens());
}

void Parser::useDimenison()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::read()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::relational()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::returnExpression()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::returnType()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::sign()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::statement()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::term()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::type()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::useParameters()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::variables()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkToken(TOKEN_IDEN);
}

void Parser::checkToken(TokenType_t expectedToken)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getToken() != expectedToken)
    {
      m_errorReporter->writeSyntaxError(expectedToken,
          m_currentToken.getToken(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

void Parser::checkLexeme(const string& expectedLexeme)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    ++m_tokenNo;
    if (m_currentToken.getLexeme().compare(expectedLexeme) != 0)
    {
      m_errorReporter->writeSyntaxError(expectedLexeme,
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

