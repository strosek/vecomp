/* Definition of member functions of class Parser */


#include "../headers/Parser.hpp"

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
#ifdef DEBUG
  cout << ":: Scanning ::" << endl;
#endif
  m_scanner.scan();

  if (m_errorReporter->getErrors() >= m_maxErrors)
    return; 

#ifdef DEBUG
  cout << ":: Parsing ::" << endl;
#endif
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

  checkLexeme("{", isLookedForward);
  advanceToken();
  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    statements(false);
  }
  checkLexeme("}", true);
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
#ifdef DEBUG
  cout << ":: entering constantsDeclaration() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("const", true);

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      constant(false);
      advanceToken();
      // TODO: ignore extra newlines
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);

    checkLexeme(")", true);
  }
  else
  {
    constant(true);
  }
}

void Parser::constant(bool isLookedForward)
{
#ifdef DEBUG
  cout << ":: entering constant() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  checkToken(TOKEN_IDEN, true);

  checkToken(TOKEN_ASSIGNOP, false);

  checkLiteral(false);
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
#ifdef DEBUG
  cout << ":: entering functionDeclaration() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("funcion", true);
  checkToken(TOKEN_IDEN, false);
  checkLexeme("(", false);
  parameterList(false);
  checkLexeme(")", true);
  checkNativeDataType(false);
  block();
#ifdef DEBUG
  cout << ":: exiting functionDeclaration() ::" << endl;
#endif
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
#ifdef DEBUG
  cout << ":: entering import() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("importar", isLookedForward);

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      // TODO: if after newline there is parentheses, exit loop
      checkToken(TOKEN_STRING, false);
      advanceToken();
      // TODO: ignore extra newlines
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);

#ifdef DEBUG
    cout << ":: current lexeme: " << m_currentToken.getLexeme() << endl;
#endif
    checkLexeme(")", true);
  }
  else
  {
    checkToken(TOKEN_STRING, true);
    checkToken(TOKEN_NEWLINE, false);
  }
#ifdef DEBUG
  cout << ":: exit import() ::" << endl;
#endif
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
#ifdef DEBUG
  cout << ":: entering parameterList() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  advanceToken();
  if (m_currentToken.getLexeme().compare(")") == 0)
  {
    return;
  }
  else
  {
    checkToken(TOKEN_IDEN, true);
    advanceToken();
  }
#ifdef DEBUG
  cout << ":: exit parameterList() ::" << endl;
#endif
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
#ifdef DEBUG
  cout << ":: entering program() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("paquete", false);

  checkLexeme("principal", false);

  ignoreNewLines(false);

  do
  {
#ifdef DEBUG
    cout << ":: current lexeme: " << m_currentToken.getLexeme() << endl;
#endif
    if (m_currentToken.getLexeme().compare("importar") == 0)
    {
      import(true);
    }
    else if (m_currentToken.getLexeme().compare("var") == 0)
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
    ignoreNewLines(false);
  } while (m_currentToken.getLexeme().compare("var") == 0       ||
           m_currentToken.getLexeme().compare("importar")  == 0 ||
           m_currentToken.getLexeme().compare("const")  == 0    ||
           m_currentToken.getLexeme().compare("funcion")  == 0);
#ifdef DEBUG
  cout << ":: exiting program() ::" << endl;
#endif
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

void Parser::statements(bool isLookedForward)
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
#ifdef DEBUG
  cout << ":: entering variablesDeclaration() ::" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("var", true);

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      variablesList(false);
      advanceToken();
      // TODO: ignore extra newlines
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
#ifdef DEBUG
  cout << ":: entering variablesList() ::" << endl;
#endif
  do
  {
    checkToken(TOKEN_IDEN, isLookedForward);
    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      expression(false);
      checkLexeme("]", true);
    }
    advanceToken();
  } while (m_currentToken.getLexeme().compare(",") == 0);

  checkNativeDataType(true);
#ifdef DEBUG
  cout << ":: exit variablesList() ::" << endl;
#endif
}

void Parser::checkToken(TokenType_t expectedToken, bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    if (!isLookedForward)
    {
      advanceToken();
    }

#ifdef DEBUG
      cout << ":: checking for " <<
          TokenLexeme::getTokenString(expectedToken) << ", got: " <<
          TokenLexeme::getTokenString(m_currentToken.getToken()) << ", " <<
          m_currentToken.getLexeme() << endl;
#endif
    if (m_currentToken.getToken() != expectedToken)
    {
      m_errorReporter->writeSyntaxError(expectedToken,
          m_currentToken.getToken(), m_currentToken.getLexeme(),
          m_currentToken.getLine(), m_currentToken.getRow());
    }
  }
}

void Parser::checkLexeme(const string& expectedLexeme, bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    if (!isLookedForward)
    {
      advanceToken();
    }

#ifdef DEBUG
      cout << ":: checking for " << expectedLexeme << ", got: " <<
          m_currentToken.getLexeme() << endl;
#endif
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
      advanceToken();
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

void Parser::checkLiteral(bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    if (!isLookedForward)
    {
      advanceToken();
    }

    if (m_currentToken.getToken() != TOKEN_DEC &&
        m_currentToken.getToken() != TOKEN_OCT &&
        m_currentToken.getToken() != TOKEN_HEX &&
        m_currentToken.getToken() != TOKEN_FLOAT &&
        m_currentToken.getToken() != TOKEN_STRING &&
        m_currentToken.getToken() != TOKEN_CHARCONST)
    {
      m_errorReporter->writeSyntaxError("constante",
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

void Parser::ignoreNewLines(bool isLookedForward)
{
#ifdef DEBUG
  cout << ":: entering ignoreNewLines() ::" << endl;
#endif
  if (!isLookedForward)
  {
    advanceToken();
  }

  while (m_currentToken.getToken() == TOKEN_NEWLINE)
  {
    advanceToken();
  }
#ifdef DEBUG
  cout << ":: exit ignoreNewLines() ::" << endl;
#endif
}

void Parser::advanceToken()
{
  m_currentToken = m_scanner.getNextTokenLexeme();
}

