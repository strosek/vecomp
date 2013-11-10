/* Definition of member functions of class Parser */


#include "../headers/Parser.hpp"

#include <string>
#include <cstdlib>
#ifdef DEBUG
# include <iostream>
#endif

using namespace std;

Parser::Parser(FileReader* fileReader, ErrorReporter* errorReporter) :
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
    cout << "expected tokens: " << m_scanner.getTokensProcessed() <<
        ", got: " << m_scanner.getMaxTokens() << endl;
#endif
  if (m_scanner.getTokensProcessed() != m_scanner.getMaxTokens())
  {
    m_errorReporter->writeError(
        "codigo incompleto, cantidad de tokens incongruente");
  }
}

void Parser::andoperator(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::assign(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::block(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::caseStatement(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::command(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::constantsDeclaration(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::dimension(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::exponent(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::expression(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::forStatement(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::functionDeclaration(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::functionSign(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::ifStatement(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::import(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::multiplication(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::notOperator(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::parameterList(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::parameters(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::print(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::program()
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("paquete", false);

  checkLexeme("principal", false);

  do
  {
    m_currentToken = m_scanner.getNextTokenLexeme();
    if (m_currentToken.getLexeme().compare("var") == 0)
    {
      variablesDeclaration(true);
    }
    else if (m_currentToken.getLexeme().compare("const") == 0)
    {
      constantsDeclaration(true);
    }
    else if (m_currentToken.getLexeme().compare("funcion") == 0)
    {
      functionDeclaration(true);
    }
  } while ((m_currentToken.getLexeme().compare("var")   ||
            m_currentToken.getLexeme().compare("const") ||
            m_currentToken.getLexeme().compare("funcion")) &&
            m_scanner.getTokensProcessed() < m_scanner.getMaxTokens());
}

void Parser::useDimenison(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::read(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::relational(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::returnExpression(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::returnType(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::sign(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::statement(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::term(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::type(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::useParameters(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
}

void Parser::variablesDeclaration(bool isLookedForward)
{
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("var", true);

  m_currentToken = m_scanner.getNextTokenLexeme();
  if (m_currentToken.getLexeme().compare("("))
  {
    do
    {
      variablesList(false);
      m_currentToken = m_scanner.getNextTokenLexeme();
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);
    checkLexeme(")", true);
  }
  else
  {
    variablesList(true);
  }
}

void Parser::variablesList(bool isLookedForward)
{
  do
  {
    checkToken(TOKEN_IDEN, isLookedForward);
    m_currentToken = m_scanner.getNextTokenLexeme();
  } while (m_currentToken.getLexeme().compare(","));
  
}

void Parser::checkToken(TokenType_t expectedToken, bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    if (!isLookedForward)
    {
      m_currentToken = m_scanner.getNextTokenLexeme();
    }

    if (m_currentToken.getToken() != expectedToken)
    {
      m_errorReporter->writeSyntaxError(expectedToken,
          m_currentToken.getToken(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

void Parser::checkLexeme(const string& expectedLexeme, bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    if (!isLookedForward)
    {
      m_currentToken = m_scanner.getNextTokenLexeme();
    }

    if (m_currentToken.getLexeme().compare(expectedLexeme) != 0)
    {
      m_errorReporter->writeSyntaxError(expectedLexeme,
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

void Parser::checkNativeDataType(bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    if (!isLookedForward)
    {
      m_currentToken = m_scanner.getNextTokenLexeme();
    }

    if (m_currentToken.getLexeme().compare("entero") != 0 &&
        m_currentToken.getLexeme().compare("real") != 0   &&
        m_currentToken.getLexeme().compare("logico") != 0 &&
        m_currentToken.getLexeme().compare("alfabetico") != 0)
    {
      m_errorReporter->writeSyntaxError("tipo_dato",
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

