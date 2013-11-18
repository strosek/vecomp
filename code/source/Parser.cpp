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
  cout << "::: Scanning :::::::::::::::::::::::::::::::::::::::::::::" << endl;
#endif
  m_scanner.scan();

  if (m_errorReporter->getErrors() >= m_maxErrors)
    return; 

#ifdef DEBUG
  cout << "::: Parsing ::::::::::::::::::::::::::::::::::::::::::::::" << endl;
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

void Parser::andOperation(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering andoperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    // TODO: define look-forward.
    notOperation(true);
  } while (m_currentToken.getLexeme().compare("&&") == 0);

#ifdef DEBUG
  cout << "::: exit andOperation()" << endl;
#endif
}

void Parser::argumentsList(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering argumentsList()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    expression(isLookedForward);
    // TODO: not sure if it will be forward looked.
    advanceToken();
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare(",") == 0);

#ifdef DEBUG
  cout << "::: exit argumentsList()" << endl;
#endif
}

void Parser::assign(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering assign()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  checkToken(TOKEN_ASSIGNOP, true);
  expression(false);
#ifdef DEBUG
  cout << "::: exit assign()" << endl;
#endif
}

void Parser::block(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering block()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("{", isLookedForward);
  advanceToken();
  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    statements(false);
  }
  checkLexeme("}", true);
#ifdef DEBUG
  cout << "::: exit block()" << endl;
#endif
}

void Parser::caseStatement(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering caseStatement()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit caseStatement()" << endl;
#endif
}

void Parser::command(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering command()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    advanceToken();
    if (m_currentToken.getToken() == TOKEN_ASSIGNOP)
    {
      assign(true);
    }
    else if (m_currentToken.getLexeme().compare("(") == 0)
    {
      functionCall(true);
    }
  }
  else if (m_currentToken.getLexeme().compare("si") == 0)
  {
    ifStatement(true);
  }
  else if (m_currentToken.getLexeme().compare("desde") == 0)
  {
    forStatement(true);
  }
  else if (m_currentToken.getLexeme().compare("caso") == 0)
  {
    caseStatement(true);
  }
  else
  {
    m_errorReporter->writeSyntaxError(
        "identificador, delimitador, si, desde o caso",
        m_currentToken.getLexeme(), m_currentToken.getLine(),
        m_currentToken.getRow());
  }
#ifdef DEBUG
  cout << "::: exit command()" << endl;
#endif
}

void Parser::constantsDeclaration(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering constantsDeclaration()" << endl;
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
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);

    checkLexeme(")", true);
  }
  else
  {
    constant(true);
  }
#ifdef DEBUG
  cout << "::: exit constantsDeclaration()" << endl;
#endif
}

void Parser::constant(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering constant()" << endl;
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
#ifdef DEBUG
  cout << "::: exit constant()" << endl;
#endif
}

void Parser::dimension(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering dimension()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit dimension()" << endl;
#endif
}

void Parser::exponent(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering exponent()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    sign(true);
  } while (m_currentToken.getLexeme().compare("^") == 0);
#ifdef DEBUG
  cout << "::: exit exponent()" << endl;
#endif
}

void Parser::expression(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering expression()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    // TODO: define look-forward
    andOperation(true);
  } while (m_currentToken.getLexeme().compare("||") == 0);

#ifdef DEBUG
  cout << "::: exit expression()" << endl;
#endif
}

void Parser::forStatement(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering forStatement()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit forStatement()" << endl;
#endif
}

void Parser::functionCall(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering functionCall()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  checkLexeme("(", true);
  
  advanceToken();
  if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    argumentsList(true);
  }
  checkLexeme(")", true);
#ifdef DEBUG
  cout << "::: exit functionCall()" << endl;
#endif
}

void Parser::functionDeclaration(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering functionDeclaration()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("funcion", true);
  checkToken(TOKEN_IDEN, false);
  checkLexeme("(", false);
  parameterList(false);
  checkLexeme(")", true);
  checkNativeDataType(false);
  block(false);
#ifdef DEBUG
  cout << "::: exit functionDeclaration()" << endl;
#endif
}

void Parser::functionSign(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering functionSign()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (m_currentToken.getLexeme().compare("-") == 0)
  {
    checkLexeme("-", true);
    term(false);
  }
  else
  {
    term(false);
  }
#ifdef DEBUG
  cout << "::: exit functionSign()" << endl;
#endif
}

void Parser::ifStatement(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering ifStatement()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit ifStatement()" << endl;
#endif
}

void Parser::import(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering import()" << endl;
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
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    checkLexeme(")", true);
  }
  else
  {
    checkToken(TOKEN_STRING, true);
    checkToken(TOKEN_NEWLINE, false);
  }
#ifdef DEBUG
  cout << "::: exit import() ::" << endl;
#endif
}

void Parser::multiplication(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering multiplication()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    exponent(true);
  } while (m_currentToken.getLexeme().compare("*") == 0 ||
           m_currentToken.getLexeme().compare("/") == 0 ||
           m_currentToken.getLexeme().compare("%") == 0);
#ifdef DEBUG
  cout << "::: exit multiplication()" << endl;
#endif
}

void Parser::notOperation(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering notOperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  if (m_currentToken.getLexeme().compare("!") == 0)
  {
    checkLexeme("!", true);
    relationalOperation(false);
  }
  relationalOperation(true);
#ifdef DEBUG
  cout << "::: exit notOperation()" << endl;
#endif
}

void Parser::parameterList(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering parameterList()" << endl;
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
  cout << "::: exit parameterList()" << endl;
#endif
}

void Parser::parameters(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering parameters()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit parameters()" << endl;
#endif
}

void Parser::print(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering print()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit print()" << endl;
#endif
}

void Parser::program()
{
#ifdef DEBUG
  cout << "::: entering program()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("paquete", false);

  checkLexeme("principal", false);

  ignoreNewLines(false);

  do
  {
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
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare("var") == 0       ||
           m_currentToken.getLexeme().compare("importar")  == 0 ||
           m_currentToken.getLexeme().compare("const")  == 0    ||
           m_currentToken.getLexeme().compare("funcion")  == 0);
#ifdef DEBUG
  cout << "::: exiting program()" << endl;
#endif
}

void Parser::useDimension(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering useDimension()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit useDimension()" << endl;
#endif
}

void Parser::read(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering read()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit read()" << endl;
#endif
}

void Parser::relationalOperation(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering relationalOperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    sumOperation(isLookedForward);
  } while (m_currentToken.getToken() == TOKEN_RELOP);

#ifdef DEBUG
  cout << "::: exit relationalOperation()" << endl;
#endif
}

void Parser::returnExpression(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering returnExpression()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit returnExpression()" << endl;
#endif
}

void Parser::returnType(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering returnType()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit returnType()" << endl;
#endif
}

void Parser::sign(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering sign()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit sign()" << endl;
#endif
}

void Parser::statements(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering statements()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }
  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    do
    {
      command(true);
      advanceToken();
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_IDEN               ||
             m_currentToken.getLexeme().compare("si") == 0         ||
             m_currentToken.getLexeme().compare("desde") == 0      ||
             m_currentToken.getLexeme().compare("caso") == 0       ||
             m_currentToken.getLexeme().compare("interrumpe") == 0 ||
             m_currentToken.getLexeme().compare("continua") == 0);
  }
#ifdef DEBUG
  cout << "::: exit statements()" << endl;
#endif
}

void Parser::sumOperation(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering sumOperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    multiplication(true);
  } while (m_currentToken.getLexeme().compare("+") == 0 ||
           m_currentToken.getLexeme().compare("-") == 0);

#ifdef DEBUG
  cout << "::: exit sumOperation()" << endl;
#endif
}

void Parser::term(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering term()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    checkLexeme("(", true);
    expression(false);
    checkLexeme(")", false);
  }
  else if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      useDimension(true);
    }
    else if (m_currentToken.getLexeme().compare("(") == 0)
    {
      useParameters(true);
    }
  }
  else 
  {
    checkLiteral(true);
  }
#ifdef DEBUG
  cout << "::: exit term()" << endl;
#endif
}

void Parser::type(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering type()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit type()" << endl;
#endif
}

void Parser::useParameters(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering useParameters()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
#ifdef DEBUG
  cout << "::: exit useParameters()" << endl;
#endif
}

void Parser::variablesDeclaration(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering variablesDeclaration()" << endl;
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
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);
    checkLexeme(")", true);
  }
  else
  {
    variablesList(true);
  }
#ifdef DEBUG
  cout << "::: exit variablesDeclaration()" << endl;
#endif
}

void Parser::variablesList(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering variablesList()" << endl;
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
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare(",") == 0);

  checkNativeDataType(true);
#ifdef DEBUG
  cout << "::: exit variablesList()" << endl;
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
      cout << "::: checking for " <<
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
      cout << "::: checking for " << expectedLexeme << ", got: " <<
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

#ifdef DEBUG
      cout << "::: checking for native data type , got: " <<
          m_currentToken.getLexeme() << endl;
#endif
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

#ifdef DEBUG
      cout << "::: checking for constant literal, got: " <<
          m_currentToken.getLexeme() << endl;
#endif
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
  cout << "::: entering ignoreNewLines()" << endl;
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
  cout << "::: exit ignoreNewLines()" << endl;
#endif
}

void Parser::advanceToken()
{
#ifdef DEBUG
  cout << "::: manual token advance" << endl;
#endif
  m_currentToken = m_scanner.getNextTokenLexeme();
}

