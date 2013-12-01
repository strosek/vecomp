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

void Parser::andOperation(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering andoperation()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  do
  {
    notOperation(true);
  } while (m_currentToken.getLexeme().compare("&&") == 0);

#ifdef DEBUG
  cout << "::: exit andOperation()" << endl;
#endif
}

void Parser::argumentsList(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering argumentsList()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  do
  {
    expression(isLookedForward);
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
  cout << "::: entering assign()";
  cout << " with isLookedForward=" << isLookedForward << endl;
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
  cout << "::: entering block()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("{", isLookedForward);
  advanceToken();
  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    statements(true);
  }
  ignoreNewLines(true);
  checkLexeme("}", true);
#ifdef DEBUG
  cout << "::: exit block()" << endl;
#endif
}

void Parser::caseStatement(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering caseStatement()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
  
  checkLexeme("caso", isLookedForward);
  checkToken(TOKEN_IDEN, false);
  checkLexeme("{", false);

  do
  {
    advanceToken();
    do
    {
      if (m_currentToken.getLexeme().compare("valor") == 0)
      {
        checkLexeme("valor", true);
        checkLiteral(false);
      }
      else if (m_currentToken.getLexeme().compare("defecto") == 0)
      {
        checkLexeme("defecto", true);
      }
      checkLexeme(":", false);
      advanceToken();
    } while (m_currentToken.getLexeme().compare("valor") == 0 ||
             m_currentToken.getLexeme().compare("defecto") == 0);

    statements(true);
  } while (m_currentToken.getLexeme().compare("valor") == 0 ||
           m_currentToken.getLexeme().compare("defecto") == 0);

#ifdef DEBUG
  cout << "::: exit caseStatement()" << endl;
#endif
}

void Parser::command(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering command()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }
#ifdef DEBUG
  cout << "checking for command first word, got: " <<
      m_currentToken.getLexeme()<< endl;
#endif
  if (m_currentToken.getToken() == TOKEN_IDEN)
  {
    if (m_currentToken.getLexeme().compare("con") == 0)
    {
      read(true);
    }
    else if (m_currentToken.getLexeme().compare("fmt") == 0)
    {
      print(true);
    }
    else
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
  else if (m_currentToken.getLexeme().compare("regresa") == 0)
  {
    returnExpression(true);
  }
  else if (m_currentToken.getLexeme().compare("var") == 0)
  {
    variablesDeclaration(true);
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

void Parser::constantsDeclaration(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering constantsDeclaration()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("const", isLookedForward);

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      constant(false);
      advanceToken();
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
  cout << "::: entering constant()";
  cout << " with isLookedForward=" << isLookedForward << endl;
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
  cout << "::: entering dimension()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("[", isLookedForward);
  expression(false);
  checkLexeme("]", false);

  advanceToken();
  while (m_currentToken.getLexeme().compare("[") == 0)
  {
    checkLexeme("[", true);
    expression(false);
    checkLexeme("]", false);
    advanceToken();
  }

#ifdef DEBUG
  cout << "::: exit dimension()" << endl;
#endif
}

void Parser::exponent(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering exponent()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  sign(true);

  if (m_currentToken.getLexeme().compare("^") == 0)
  {
    sign(false);
  }

#ifdef DEBUG
  cout << "::: exit exponent()" << endl;
#endif
}

void Parser::expression(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering expression()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  bool isFirstIteration = true;
  do
  {
    if (isFirstIteration)
    {
      andOperation(true);
      isFirstIteration = false;
    }
    else
    {
      andOperation(false);
    }
  } while (m_currentToken.getLexeme().compare("||") == 0);

#ifdef DEBUG
  cout << "::: exit expression()" << endl;
#endif
}

void Parser::forStatement(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering forStatement()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
  
  checkLexeme("desde", isLookedForward);
  checkLexeme("(", false);

  assign(false);
  while (m_currentToken.getLexeme().compare(",") == 0)
  {
    assign(true);
    advanceToken();
  }
  checkLexeme(";", true);

  advanceToken();
  if (m_currentToken.getLexeme().compare(";") != 0)
  {
    expression(true);
  }
  checkLexeme(";", false);

  assign(false);
  while (m_currentToken.getLexeme().compare(",") == 0)
  {
    assign(true);
    advanceToken();
  }
  checkLexeme(")", true);
  block(false);

#ifdef DEBUG
  cout << "::: exit forStatement()" << endl;
#endif
}

void Parser::functionCall(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering functionCall()";
  cout << " with isLookedForward=" << isLookedForward << endl;
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
  advanceToken();
#ifdef DEBUG
  cout << "::: exit functionCall()" << endl;
#endif
}

void Parser::functionDeclaration(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering functionDeclaration()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("funcion", isLookedForward);
  checkToken(TOKEN_IDEN, false);
  checkLexeme("(", false);
  parameterList(false);
  checkLexeme(")", false);
  returnType(false);
  block(false);
#ifdef DEBUG
  cout << "::: exit functionDeclaration()" << endl;
#endif
}

void Parser::ifStatement(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering ifStatement()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("si", isLookedForward);
  expression(false);
  block(true);

  ignoreNewLines(false);
  if (m_currentToken.getLexeme().compare("sino") == 0)
  {
    checkLexeme("sino", true);
    block(false);
  }

#ifdef DEBUG
  cout << "::: exit ifStatement()" << endl;
#endif
}

void Parser::import(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering import()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("importar", isLookedForward);

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      checkToken(TOKEN_STRING, false);
      advanceToken();
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
  cout << "::: entering multiplication()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  // TODO: should this be written as 1, while 2..n?
  bool isFirstIteration = true;
  do
  {
    if (isFirstIteration)
    {
      exponent(true);
      isFirstIteration = false;
    }
    else
    {
      exponent(false);
    }
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
  cout << "::: entering notOperation()";
  cout << " with isLookedForward=" << isLookedForward << endl;
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
  cout << "::: entering parameterList()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  if (m_currentToken.getLexeme().compare(")") != 0)
  {
    bool isFirstIteration = true;
    do
    {
      if (isFirstIteration)
      {
        checkToken(TOKEN_IDEN, true);
        isFirstIteration = false;
      }
      else
      {
        checkToken(TOKEN_IDEN, false);
      }
      advanceToken();
    } while (m_currentToken.getLexeme().compare(",") == 0);
    checkNativeDataType(true);
  }
#ifdef DEBUG
  cout << "::: exit parameterList()" << endl;
#endif
}

void Parser::print(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering print()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("fmt", isLookedForward);
  checkLexeme(".", false);

  advanceToken();
  if (m_currentToken.getLexeme().compare("Imprime") != 0 ||
      m_currentToken.getLexeme().compare("Imprimenl") != 0)
  {
    m_errorReporter->writeSyntaxError("Imprime o Imprimenl",
        m_currentToken.getLexeme(), m_currentToken.getLine(),
        m_currentToken.getRow()); 
  }
  
  checkLexeme("(", false);
  argumentsList(false);
  checkLexeme(")", true);

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

void Parser::read(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering read()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("fmt", isLookedForward);
  checkLexeme(".", false);
  checkLexeme("Lee", false);
  checkLexeme("(", false);
  argumentsList(false);
  checkLexeme(")", true);

#ifdef DEBUG
  cout << "::: exit read()" << endl;
#endif
}

void Parser::relationalOperation(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering relationalOperation()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  sumOperation(isLookedForward);
  if (m_currentToken.getToken() == TOKEN_RELOP)
  {
    sumOperation(false);
  }

#ifdef DEBUG
  cout << "::: exit relationalOperation()" << endl;
#endif
}

void Parser::returnExpression(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering returnExpression()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("regresa", isLookedForward);

  advanceToken();
  if (m_currentToken.getLexeme().compare(";") != 0 &&
      m_currentToken.getToken() != TOKEN_NEWLINE)
  {
    expression(true);
  }

#ifdef DEBUG
  cout << "::: exit returnExpression()" << endl;
#endif
}

void Parser::returnType(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering returnType()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;
  
  if (!isLookedForward)
  {
    advanceToken();
  }

  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    checkLexeme("(", true);
    advanceToken();
    if (m_currentToken.getToken() == TOKEN_IDEN)
    {
      do
      {
        checkToken(TOKEN_IDEN, true);
        variablesList(false);
        checkNativeDataType(true);
        advanceToken();
      } while (m_currentToken.getLexeme().compare(",") == 0);
      checkLexeme(")", true);
    }
    else
    {
      checkNativeDataType(true);
      checkLexeme(")", false);
    }
  }
  else if (m_currentToken.getLexeme().compare("entero") == 0 ||
           m_currentToken.getLexeme().compare("real") == 0   ||
           m_currentToken.getLexeme().compare("logico") == 0 ||
           m_currentToken.getLexeme().compare("alfabetico") == 0)
  {
    checkNativeDataType(true);
  }

#ifdef DEBUG
  cout << "::: exit returnType()" << endl;
#endif
}

void Parser::sign(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering sign()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  if (m_currentToken.getLexeme().compare("-") == 0)
  {
    checkLexeme("-", true);
  }
  term(true);

#ifdef DEBUG
  cout << "::: exit sign()" << endl;
#endif
}

void Parser::statements(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering statements()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  if (m_currentToken.getLexeme().compare("}") != 0)
  {
    ignoreNewLines(true);
    do
    {
      command(true);
#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    } while (m_currentToken.getToken() == TOKEN_IDEN               ||
             m_currentToken.getLexeme().compare("var") == 0        ||
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
  cout << "::: entering sumOperation()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

  bool isFirstIteration = true;
  do
  {
    if (isFirstIteration)
    {
      multiplication(true);
      isFirstIteration = false;
    }
    else
    {
      multiplication(false);
    }
  } while (m_currentToken.getLexeme().compare("+") == 0 ||
           m_currentToken.getLexeme().compare("-") == 0);

#ifdef DEBUG
  cout << "::: exit sumOperation()" << endl;
#endif
}

void Parser::term(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering term()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  if (!isLookedForward)
  {
    advanceToken();
  }

#ifdef DEBUG
  cout << "checking term with lexeme: " << m_currentToken.getLexeme() << endl;
#endif
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
      dimension(true);
      advanceToken();
    }
    else if (m_currentToken.getLexeme().compare("(") == 0)
    {
      functionCall(true);
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

void Parser::variablesDeclaration(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering variablesDeclaration()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (m_errorReporter->getErrors() >= m_maxErrors)
    return;

  checkLexeme("var", isLookedForward);

  advanceToken();
  if (m_currentToken.getLexeme().compare("(") == 0)
  {
    do
    {
      variablesList(false);
      advanceToken();
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
  advanceToken();
#ifdef DEBUG
  cout << "::: exit variablesDeclaration()" << endl;
#endif
}

void Parser::variablesList(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: entering variablesList()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (!isLookedForward)
  {
    advanceToken();
  }

  bool isFirstIteration = true;
  do
  {
    if (isFirstIteration)
    {
      checkToken(TOKEN_IDEN, true);
      isFirstIteration = false;
    }
    else
    {
      checkToken(TOKEN_IDEN, false);
    }

    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      do
      {
        expression(false);
        checkLexeme("]", true);
        advanceToken();
      } while (m_currentToken.getLexeme().compare("[") == 0);
    }

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
    if (expectedToken != TOKEN_NEWLINE)
    {
      ignoreNewLines(isLookedForward);
    }
    else if (!isLookedForward)
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
    ignoreNewLines(isLookedForward);

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
    ignoreNewLines(isLookedForward);

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

void Parser::checkLiteral(bool isLookedForward)
{
  if (m_errorReporter->getErrors() < m_maxErrors)
  {
    ignoreNewLines(isLookedForward);

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

void Parser::ignoreNewLines(bool isLookedForward)
{
#ifdef DEBUG
  cout << "::: ignoring newlines()";
  cout << " with isLookedForward=" << isLookedForward << endl;
#endif
  if (!isLookedForward)
  {
    advanceToken();
  }

  while (m_currentToken.getToken() == TOKEN_NEWLINE ||
         m_currentToken.getLexeme().compare(";") == 0)
  {
    advanceToken();
  }
#ifdef DEBUG
  cout << "::: stopped ignoring newlines()" << endl;
#endif
}

void Parser::advanceToken()
{
  m_currentToken = m_scanner.getNextTokenLexeme();
#ifdef DEBUG
  cout << "::: token advance to: " << m_currentToken.getLexeme() << endl;
#endif
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

