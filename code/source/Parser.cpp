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
  m_maxErrors(1)
{
  m_maxErrors = m_errorReporter->getMaxErrors();
}

void Parser::parse()
{
#ifdef DEBUG
  cout << "::: Scanning :::::::::::::::::::::::::::::::::::::::::::::" << endl;
#endif
  m_scanner.scan();
#ifdef DEBUG
    cout << "Total tokens: " << m_scanner.getMaxTokens() << endl;
#endif

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

void Parser::andOperation()
{
#ifdef DEBUG
  cout << "::: entering andoperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    notOperation();
  } while (m_currentToken.getLexeme().compare("&&") == 0);

#ifdef DEBUG
  cout << "::: exit andOperation()" << endl;
#endif
}

void Parser::argumentsList()
{
#ifdef DEBUG
  cout << "::: entering argumentsList()" << endl;
#endif

  do
  {
    expression();
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare(",") == 0);

#ifdef DEBUG
  cout << "::: exit argumentsList()" << endl;
#endif
}

void Parser::assign()
{
#ifdef DEBUG
  cout << "::: entering assign()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkToken(TOKEN_ASSIGNOP);
  expression();
#ifdef DEBUG
  cout << "::: exit assign()" << endl;
#endif
}

void Parser::block()
{
#ifdef DEBUG
  cout << "::: entering block()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("{");
  advanceToken();
  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    statements();
  }
  ignoreNewLines();
  checkLexeme("}");
#ifdef DEBUG
  cout << "::: exit block()" << endl;
#endif
}

void Parser::caseStatement()
{
#ifdef DEBUG
  cout << "::: entering caseStatement()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
  
  checkLexeme("caso");
  checkToken(TOKEN_IDEN);
  checkLexeme("{");

  do
  {
    advanceToken();
    do
    {
      if (m_currentToken.getLexeme().compare("valor") == 0)
      {
        checkLexeme("valor");
        checkLiteral();
      }
      else if (m_currentToken.getLexeme().compare("defecto") == 0)
      {
        checkLexeme("defecto");
      }
      checkLexeme(":");
      advanceToken();
    } while (m_currentToken.getLexeme().compare("valor") == 0 ||
             m_currentToken.getLexeme().compare("defecto") == 0);

    statements();
  } while (m_currentToken.getLexeme().compare("valor") == 0 ||
           m_currentToken.getLexeme().compare("defecto") == 0);

#ifdef DEBUG
  cout << "::: exit caseStatement()" << endl;
#endif
}

void Parser::command()
{
#ifdef DEBUG
  cout << "::: entering command()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

#ifdef DEBUG
  cout << "checking for command first word, got: " <<
      m_currentToken.getLexeme()<< endl;
#endif
  if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    if (m_currentToken.getLexeme().compare("con") == 0)
    {
      read();
    }
    else if (m_currentToken.getLexeme().compare("fmt") == 0)
    {
      print();
    }
    else
    {
      advanceToken();
      if (m_currentToken.getToken() == TOKEN_ASSIGNOP)
      {
        assign();
      }
      else if (m_currentToken.getLexeme().compare("(") == 0)
      {
        functionCall();
      }
    }
  }
  else if (m_currentToken.getLexeme().compare("si") == 0)
  {
    ifStatement();
  }
  else if (m_currentToken.getLexeme().compare("desde") == 0)
  {
    forStatement();
  }
  else if (m_currentToken.getLexeme().compare("caso") == 0)
  {
    caseStatement();
  }
  else if (m_currentToken.getLexeme().compare("regresa") == 0)
  {
    returnExpression();
  }
  else if (m_currentToken.getLexeme().compare("var") == 0)
  {
    variablesDeclaration();
  }
  else
  {
    m_errorReporter->writeSyntaxError(
        "identificador, delimitador, si, desde, caso o regresa",
        m_currentToken.getLexeme(), m_currentToken.getLine(),
        m_currentToken.getRow());
  }
#ifdef DEBUG
  cout << "::: exit command()" << endl;
#endif
}

void Parser::constantsDeclaration()
{
#ifdef DEBUG
  cout << "::: entering constantsDeclaration()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("const");

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      constant();
      advanceToken();
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);
  }
  else
  {
    constant();
  }
#ifdef DEBUG
  cout << "::: exit constantsDeclaration()" << endl;
#endif
}

void Parser::constant()
{
#ifdef DEBUG
  cout << "::: entering constant()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkToken(TOKEN_IDEN);

  checkToken(TOKEN_ASSIGNOP);

  checkLiteral();
#ifdef DEBUG
  cout << "::: exit constant()" << endl;
#endif
}

void Parser::dimension()
{
#ifdef DEBUG
  cout << "::: entering dimension()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do {
    checkLexeme("[");
    advanceToken();
    expression();
    m_scanner.moveTokenBackward();
    checkLexeme("]");
    advanceToken();
  } while (m_currentToken.getLexeme().compare("[") == 0);

  m_scanner.moveTokenBackward();

#ifdef DEBUG
  cout << "::: exit dimension()" << endl;
#endif
}

void Parser::exponent()
{
#ifdef DEBUG
  cout << "::: entering exponent()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  sign();

  if (m_currentToken.getLexeme().compare("^") == 0)
  {
    sign();
  }

#ifdef DEBUG
  cout << "::: exit exponent()" << endl;
#endif
}

void Parser::expression()
{
#ifdef DEBUG
  cout << "::: entering expression()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    andOperation();
  } while (m_currentToken.getLexeme().compare("||") == 0);

#ifdef DEBUG
  cout << "::: exit expression()" << endl;
#endif
}

void Parser::forStatement()
{
#ifdef DEBUG
  cout << "::: entering forStatement()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
  
  checkLexeme("desde");

  advanceToken();

  if (m_currentToken.getLexeme().compare(";") != 0)
  {
    assign();
    while (m_currentToken.getLexeme().compare(",") == 0)
    {
      assign();
    }
  }

  checkLexeme(";");

  advanceToken();
  cout << "checking for empty condition in for: " << 
      m_currentToken.getLexeme() << endl;
  if (m_currentToken.getLexeme().compare(";") != 0)
  {
    expression();
  }

  checkLexeme(";");
  advanceToken();

  if (m_currentToken.getLexeme().compare(";") != 0)
  {
    assign();
    while (m_currentToken.getLexeme().compare(",") == 0)
    {
      assign();
      advanceToken();
    }
  }
  block();
  ignoreNewLines();

#ifdef DEBUG
  cout << "::: exit forStatement()" << endl;
#endif
}

void Parser::functionCall()
{
#ifdef DEBUG
  cout << "::: entering functionCall()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("(");
  
  advanceToken();
  if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    argumentsList();
  }
  checkLexeme(")");
  advanceToken();
#ifdef DEBUG
  cout << "::: exit functionCall()" << endl;
#endif
}

void Parser::functionDeclaration()
{
#ifdef DEBUG
  cout << "::: entering functionDeclaration()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("funcion");
  checkToken(TOKEN_IDEN);
  checkLexeme("(");
  parameterList();
  checkLexeme(")");
  returnType();
  block();
  ignoreNewLines();
#ifdef DEBUG
  cout << "::: exit functionDeclaration()" << endl;
#endif
}

void Parser::ifStatement()
{
#ifdef DEBUG
  cout << "::: entering ifStatement()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("si");
  expression();
  block();
  ignoreNewLines();

  if (m_currentToken.getLexeme().compare("sino") == 0)
  {
    checkLexeme("sino");
    block();
  }

#ifdef DEBUG
  cout << "::: exit ifStatement()" << endl;
#endif
}

void Parser::import()
{
#ifdef DEBUG
  cout << "::: entering import()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("importar");

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      checkToken(TOKEN_STRING);
      advanceToken();
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);

#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  }
  else
  {
    checkToken(TOKEN_STRING);
    checkToken(TOKEN_NEWLINE);
  }
#ifdef DEBUG
  cout << "::: exit import() ::" << endl;
#endif
}

void Parser::multiplication()
{
#ifdef DEBUG
  cout << "::: entering multiplication()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    exponent();
  } while (m_currentToken.getLexeme().compare("*") == 0 ||
           m_currentToken.getLexeme().compare("/") == 0 ||
           m_currentToken.getLexeme().compare("%") == 0);

#ifdef DEBUG
  cout << "::: exit multiplication()" << endl;
#endif
}

void Parser::notOperation()
{
#ifdef DEBUG
  cout << "::: entering notOperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (m_currentToken.getLexeme().compare("!") == 0)
  {
    checkLexeme("!");
    relationalOperation();
  }
  relationalOperation();
#ifdef DEBUG
  cout << "::: exit notOperation()" << endl;
#endif
}

void Parser::parameterList()
{
#ifdef DEBUG
  cout << "::: entering parameterList()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  while (m_currentToken.getLexeme().compare(")") != 0)
  {
    do
    {
      checkToken(TOKEN_IDEN);
      advanceToken();
    } while (m_currentToken.getLexeme().compare(",") == 0);
    checkNativeDataType();
    advanceToken();
  }
#ifdef DEBUG
  cout << "::: exit parameterList()" << endl;
#endif
}

void Parser::print()
{
#ifdef DEBUG
  cout << "::: entering print()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("fmt");
  checkLexeme(".");

  advanceToken();
  if (m_currentToken.getLexeme().compare("Imprime") != 0 ||
      m_currentToken.getLexeme().compare("Imprimenl") != 0)
  {
    m_errorReporter->writeSyntaxError("Imprime o Imprimenl",
        m_currentToken.getLexeme(), m_currentToken.getLine(),
        m_currentToken.getRow()); 
  }
  
  checkLexeme("(");
  argumentsList();
  checkLexeme(")");

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

  checkLexeme("paquete");

  checkLexeme("principal");
 
  ignoreNewLines();
  do
  {
    if (m_currentToken.getLexeme().compare("importar") == 0)
    {
      import();
    }
    else if (m_currentToken.getLexeme().compare("var") == 0)
    {
      variablesDeclaration();
    }
    else if (m_currentToken.getLexeme().compare("const") == 0)
    {
      constantsDeclaration();
    }
    else if (m_currentToken.getLexeme().compare("funcion") == 0)
    {
      functionDeclaration();
    }
    ignoreNewLines();
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare("var") == 0       ||
           m_currentToken.getLexeme().compare("importar")  == 0 ||
           m_currentToken.getLexeme().compare("const")  == 0    ||
           m_currentToken.getLexeme().compare("funcion")  == 0);
#ifdef DEBUG
  cout << "::: exit program()" << endl;
#endif
}

void Parser::read()
{
#ifdef DEBUG
  cout << "::: entering read()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("fmt");
  checkLexeme(".");
  checkLexeme("Lee");
  checkLexeme("(");
  argumentsList();
  checkLexeme(")");

#ifdef DEBUG
  cout << "::: exit read()" << endl;
#endif
}

void Parser::relationalOperation()
{
#ifdef DEBUG
  cout << "::: entering relationalOperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  sumOperation();
  if (m_currentToken.getToken() == TOKEN_RELOP)
  {
    sumOperation();
  }

#ifdef DEBUG
  cout << "::: exit relationalOperation()" << endl;
#endif
}

void Parser::returnExpression()
{
#ifdef DEBUG
  cout << "::: entering returnExpression()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("regresa");

  advanceToken();
  if (m_currentToken.getLexeme().compare(";") != 0 &&
      m_currentToken.getToken() != TOKEN_NEWLINE)
  {
    expression();
  }

#ifdef DEBUG
  cout << "::: exit returnExpression()" << endl;
#endif
}

void Parser::returnType()
{
#ifdef DEBUG
  cout << "::: entering returnType()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
  
  if (m_currentToken.getLexeme().compare("{") != 0)
  {
    if (m_currentToken.getLexeme().compare("(") == 0)
    {
      checkLexeme("(");
      advanceToken();
      if (m_currentToken.getToken() == TOKEN_IDEN)
      {
        do
        {
          checkToken(TOKEN_IDEN);
          variablesList();
          checkNativeDataType();
          advanceToken();
        } while (m_currentToken.getLexeme().compare(",") == 0);
        checkLexeme(")");
      }
      else
      {
        checkNativeDataType();
        checkLexeme(")");
      }
    }
    else if (m_currentToken.getLexeme().compare("entero") == 0 ||
             m_currentToken.getLexeme().compare("real") == 0   ||
             m_currentToken.getLexeme().compare("logico") == 0 ||
             m_currentToken.getLexeme().compare("alfabetico") == 0)
    {
      checkNativeDataType();
    }
  }

#ifdef DEBUG
  cout << "::: exit returnType()" << endl;
#endif
}

void Parser::sign()
{
#ifdef DEBUG
  cout << "::: entering sign()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (m_currentToken.getLexeme().compare("-") == 0)
  {
    checkLexeme("-");
  }
  term();

#ifdef DEBUG
  cout << "::: exit sign()" << endl;
#endif
}

void Parser::statements()
{
#ifdef DEBUG
  cout << "::: entering statements()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    ignoreNewLines();
    do
    {
      command();
      ignoreNewLines();
#ifdef DEBUG
      cout << "::: current lexeme (line " << __LINE__ << "): " <<
          m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_IDEN               ||
             m_currentToken.getLexeme().compare("var") == 0        ||
             m_currentToken.getLexeme().compare("si") == 0         ||
             m_currentToken.getLexeme().compare("desde") == 0      ||
             m_currentToken.getLexeme().compare("caso") == 0       ||
             m_currentToken.getLexeme().compare("regresa") == 0    ||
             m_currentToken.getLexeme().compare("interrumpe") == 0 ||
             m_currentToken.getLexeme().compare("continua") == 0);
  }
#ifdef DEBUG
  cout << "::: exit statements()" << endl;
#endif
}

void Parser::sumOperation()
{
#ifdef DEBUG
  cout << "::: entering sumOperation()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    multiplication();
  } while (m_currentToken.getLexeme().compare("+") == 0 ||
           m_currentToken.getLexeme().compare("-") == 0);

#ifdef DEBUG
  cout << "::: exit sumOperation()" << endl;
#endif
}

void Parser::term()
{
#ifdef DEBUG
  cout << "::: entering term()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

#ifdef DEBUG
  cout << "checking term with lexeme: " << m_currentToken.getLexeme() << endl;
#endif
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    checkLexeme("(");
    expression();
    checkLexeme(")");
  }
  else if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      dimension();
      advanceToken();
    }
    else if (m_currentToken.getLexeme().compare("(") == 0)
    {
      functionCall();
      advanceToken();
    }
  }
  else if (isLiteral(m_currentToken.getToken()))
  {
    advanceToken();
  }
  else {
    m_errorReporter->writeSyntaxError("identificador o constante",
            m_currentToken.getLexeme(), m_currentToken.getLine(),
            m_currentToken.getRow());
  }
#ifdef DEBUG
  cout << "::: exit term()" << endl;
#endif
}

void Parser::variablesDeclaration()
{
#ifdef DEBUG
  cout << "::: entering variablesDeclaration()" << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("var");

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      variablesList();
      advanceToken();
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_NEWLINE);
    checkLexeme(")");
  }
  else
  {
    variablesList();
  }
  advanceToken();
#ifdef DEBUG
  cout << "::: exit variablesDeclaration()" << endl;
#endif
}

void Parser::variablesList()
{
#ifdef DEBUG
  cout << "::: entering variablesList()" << endl;
#endif
  do
  {
    checkToken(TOKEN_IDEN);

    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      m_scanner.moveTokenBackward();
      dimension();
    }

#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare(",") == 0);

  checkNativeDataType();
#ifdef DEBUG
  cout << "::: exit variablesList()" << endl;
#endif
}

void Parser::checkToken(TokenType_t expectedToken)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    advanceToken();
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

void Parser::checkLexeme(const string& expectedLexeme)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    advanceToken();
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


void Parser::checkNativeDataType()
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    advanceToken();
#ifdef DEBUG
      cout << "::: checking for native data type, got: " <<
          m_currentToken.getLexeme() << endl;
#endif
    if (!isNativeDataType(m_currentToken.getLexeme()))
    {
      m_errorReporter->writeSyntaxError("tipo_dato",
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

void Parser::checkLiteral()
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    advanceToken();
#ifdef DEBUG
      cout << "::: checking for constant literal, got: " <<
          m_currentToken.getLexeme() << endl;
#endif
    if (!isLiteral(m_currentToken.getToken()))
    {
      m_errorReporter->writeSyntaxError("constante",
          m_currentToken.getLexeme(), m_currentToken.getLine(),
          m_currentToken.getRow());
    }
  }
}

void Parser::ignoreNewLines()
{
#ifdef DEBUG
  cout << "::: ignoring newlines()" << endl;
#endif

  do
  {
    advanceToken();
  } while (m_currentToken.getToken() == TOKEN_NEWLINE ||
           m_currentToken.getLexeme().compare(";") == 0);

  m_scanner.moveTokenBackward();

#ifdef DEBUG
  cout << "::: stopped ignoring newlines()" << endl;
#endif
}

void Parser::advanceToken()
{
  m_currentToken = m_scanner.getNextTokenLexeme();
}

bool Parser::isNativeDataType(const string& lexeme)
{
    if (lexeme.compare("entero") == 0 ||
        lexeme.compare("real") == 0   ||
        lexeme.compare("logico") == 0 ||
        lexeme.compare("alfabetico") == 0)
    {
      return true;
    }
    return false;
}

bool Parser::isLiteral(TokenType_t token)
{
  if (token == TOKEN_DEC       ||
      token == TOKEN_HEX       ||
      token == TOKEN_OCT       ||
      token == TOKEN_FLOAT     ||
      token == TOKEN_CHARCONST ||
      token == TOKEN_STRING)
  {
    return true;
  }

  return false;
}

