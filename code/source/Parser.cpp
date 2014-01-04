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
  m_maxErrors(5),
  m_nTokensProcessed(0),
  m_semanticChecker(SemanticChecker(errorReporter)),
  m_currentSymbolName(),
  m_currentSymbolData(),
  m_currentSymbols()
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

  if (!m_semanticChecker.isMainPresent())
  {
    m_errorReporter->writeError("no se declaro una funcion principal");
  }

#ifdef DEBUG
  cout << "expected tokens: " << m_scanner.getTokensProcessed() <<
      ", got: " << m_scanner.getMaxTokens() << endl;

  cout << "\n::: Symbols table :::::::::::::::::::::::::::::::::::::::" << endl;
  m_semanticChecker.printSymbolsTable();
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

  bool isOperatorFound = false;
  do
  {
    isOperatorFound = false;
    expression();

    advanceToken();
    if (m_currentToken.getLexeme().compare(",") == 0)
    {
      isOperatorFound = true;
      advanceToken();
    }
  } while (isOperatorFound); 

  m_scanner.moveTokenBackwards();

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
  advanceToken();
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
    m_scanner.moveTokenBackwards();
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

  m_semanticChecker.enterSwitch();
  
  checkLexeme("caso");
  checkToken(TOKEN_IDEN);
  checkLexeme("{");

  do
  {
    ignoreNewLines();
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

    m_scanner.moveTokenBackwards();
    statements();
  } while (m_currentToken.getLexeme().compare("valor") == 0 ||
           m_currentToken.getLexeme().compare("defecto") == 0);

  checkLexeme("}");

  m_semanticChecker.exitSwitch();
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
      m_currentToken.getLexeme() << endl;
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
      // FIXME: this should not need the double forward movement.
      m_scanner.moveTokenForward();
      ++m_nTokensProcessed;
      advanceToken();

      if (m_currentToken.getToken() == TOKEN_ASSIGNOP)
      {
        m_scanner.moveTokenBackwards();

        m_semanticChecker.checkModifiable(getLastToken());

        assign();
      }
      else if (m_currentToken.getLexeme().compare("[") == 0)
      {
        m_scanner.moveTokenBackwards();
        dimension();
        assign();
      }
      else if (m_currentToken.getLexeme().compare("(") == 0)
      {
        m_scanner.moveTokenBackwards();
        functionCall();
      }
      else
      {
        m_errorReporter->writeSyntaxError(
            ":=, ( o [",
            m_currentToken.getLexeme(), m_currentToken.getLine(),
            m_currentToken.getRow());
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
  else if (m_currentToken.getLexeme().compare("interrumpe") == 0)
  {
    advanceToken();

    if (!m_semanticChecker.isInSwitch() && !m_semanticChecker.isInFor())
    {
      TokenLexeme lastToken = getLastToken();
      m_errorReporter->writeError(lastToken.getLine(), lastToken.getRow(),
            lastToken.getLexeme(),
            "interrumpe solo es permitido en ciclo desde o en caso");
    }
  }
  else if (m_currentToken.getLexeme().compare("continua") == 0)
  {
    advanceToken();
    if (!m_semanticChecker.isInFor())
    {
      TokenLexeme lastToken = getLastToken();
      m_errorReporter->writeError(lastToken.getLine(), lastToken.getRow(),
            lastToken.getLexeme(), "continua solo es permitido en ciclo desde");
    }
  }
  else
  {
    m_errorReporter->writeError(m_currentToken.getLine(),
        m_currentToken.getRow(), m_currentToken.getLexeme(),
        "inicio de comando invalido");
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

  m_currentSymbolName = getLastToken().getLexeme();
  m_currentSymbolData.scope = m_semanticChecker.getCurrentScope();
  m_currentSymbolData.isConstant = true;
  m_currentSymbolData.line = getLastToken().getLine();

  checkToken(TOKEN_ASSIGNOP);

  checkLiteral();

  m_currentSymbolData.type = getLiteralType(getLastToken().getToken());

  m_currentSymbolName += "@";
  m_currentSymbolName += m_semanticChecker.getCurrentScope();
  m_semanticChecker.addSymbol(m_currentSymbolName, m_currentSymbolData);
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
    m_currentSymbols.back().second.dimensions.push_back(0);

    checkLexeme("[");
    advanceToken();
    expression();
    checkLexeme("]");
    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      m_scanner.moveTokenBackwards();
    }
  } while (m_currentToken.getLexeme().compare("[") == 0);

  m_scanner.moveTokenBackwards();

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

  m_scanner.moveTokenBackwards();

#ifdef DEBUG
  cout << "::: exit expression()" << endl;
#endif
}

void Parser::forStatement()
{
#ifdef DEBUG
  cout << "::: entering forStatement()" << endl;
#endif
  m_semanticChecker.enterFor();

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
  else
  {
    m_scanner.moveTokenBackwards();
  }

  checkLexeme(";"); 

  advanceToken();
  if (m_currentToken.getLexeme().compare(";") != 0)
  {
    expression();
  }
  else
  {
    m_scanner.moveTokenBackwards();
  }

  checkLexeme(";");

  advanceToken();
  if (m_currentToken.getLexeme().compare("{") != 0)
  {
    assign();
    while (m_currentToken.getLexeme().compare(",") == 0)
    {
      assign();
      advanceToken();
    }
  }
  else
  {
    m_scanner.moveTokenBackwards();
  }

  block();

  m_semanticChecker.exitFor();
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
  if (m_currentToken.getToken() == TOKEN_IDEN ||
      m_currentToken.getToken() == TOKEN_STRING ||
      m_currentToken.getToken() == TOKEN_DEC ||
      m_currentToken.getToken() == TOKEN_HEX ||
      m_currentToken.getToken() == TOKEN_OCT ||
      m_currentToken.getToken() == TOKEN_FLOAT)
  {
    argumentsList();
  }
  else
  {
    m_scanner.moveTokenBackwards();
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
  if (getLastToken().getLexeme().compare("principal") == 0)
  {
    m_semanticChecker.setMainPresent(true);
  }

  m_currentSymbolName = getLastToken().getLexeme();
  m_currentSymbolName += "()@";
  m_currentSymbolName += m_semanticChecker.getCurrentScope();

  m_currentSymbolData.isFunction = true;
  m_currentSymbolData.line = getLastToken().getLine();
  m_currentSymbolData.scope = m_semanticChecker.getCurrentScope();
  m_currentSymbolData.isConstant = false;

  m_semanticChecker.enterToScope(getLastToken().getLexeme());

  checkLexeme("(");
  parameterList();
  checkLexeme(")");
  returnType();

  addSymbolAndReset(m_currentSymbolName, m_currentSymbolData);

  block();

  m_semanticChecker.exitCurrentScope();
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

  advanceToken();
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

      m_semanticChecker.addImport(getLastToken());

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

  bool isOperatorFound = false;
  do
  {
    isOperatorFound = false;
    exponent();
    if (m_currentToken.getLexeme().compare("*") == 0 ||
        m_currentToken.getLexeme().compare("/") == 0 ||
        m_currentToken.getLexeme().compare("%") == 0)
    {
      isOperatorFound = true;
      advanceToken();
    }
  } while (isOperatorFound); 
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

  string nameWithScope;
  int    nTypeParameters;

  advanceToken();
  while (m_currentToken.getLexeme().compare(")") != 0)
  {
    m_scanner.moveTokenBackwards();

    nTypeParameters = 0;
    do
    {
      ++nTypeParameters;

      checkToken(TOKEN_IDEN);

      nameWithScope = getLastToken().getLexeme();
      nameWithScope += "@";
      nameWithScope += m_semanticChecker.getCurrentScope();
      m_currentSymbols.push_back(
          make_pair(nameWithScope, SymbolData_t()));

      advanceToken();
    } while (m_currentToken.getLexeme().compare(",") == 0);

    m_scanner.moveTokenBackwards();
    checkNativeDataType();

    m_currentSymbolData.parametersList.push_back(
        make_pair(nTypeParameters,
        getTypeFromString(getLastToken().getLexeme())));

    setCurrentSymbolsData();

    advanceToken();
  }

  m_scanner.moveTokenBackwards();
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
  if (m_currentToken.getLexeme().compare("Imprime") != 0 &&
      m_currentToken.getLexeme().compare("Imprimenl") != 0)
  {
    m_errorReporter->writeSyntaxError("Imprime o Imprimenl",
        m_currentToken.getLexeme(), m_currentToken.getLine(),
        m_currentToken.getRow()); 
  }
  
  checkLexeme("(");
  advanceToken();
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

  m_semanticChecker.enterToScope("global");

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

    if (m_currentToken.getLexeme().compare(",") == 0 ||
        m_currentToken.getToken() == TOKEN_NEWLINE)
    {
      advanceToken();
    }

#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
  } while (m_currentToken.getLexeme().compare("var") == 0       ||
           m_currentToken.getLexeme().compare("importar")  == 0 ||
           m_currentToken.getLexeme().compare("const")  == 0    ||
           m_currentToken.getLexeme().compare("funcion")  == 0);

  m_semanticChecker.exitCurrentScope();
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

  checkLexeme("con");
  checkLexeme(".");
  checkLexeme("Lee");
  checkLexeme("(");
  advanceToken();
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
    advanceToken();
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
  
  advanceToken();
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
    else 
    {
      m_scanner.moveTokenBackwards();
      checkNativeDataType();

      m_currentSymbolData.type = getTypeFromString(getLastToken().getLexeme());
    }
  }
  else
  {
    m_scanner.moveTokenBackwards();
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
    m_scanner.moveTokenBackwards();
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
      if (m_currentToken.getLexeme().compare(";") == 0 ||
          m_currentToken.getToken() == TOKEN_NEWLINE)
      {
        advanceToken();
      }
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

  bool isOperatorFound = false;
  do
  {
    isOperatorFound = false;
    multiplication();
    if (m_currentToken.getLexeme().compare("+") == 0 ||
        m_currentToken.getLexeme().compare("-") == 0)
    {
      isOperatorFound = true;
      advanceToken();
    }
  } while (isOperatorFound); 

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
      m_scanner.moveTokenBackwards();
      dimension();
      advanceToken();
    }
    else if (m_currentToken.getLexeme().compare("(") == 0)
    {
      m_scanner.moveTokenBackwards();
      functionCall();
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
    m_scanner.moveTokenBackwards();
    checkLexeme(")");
  }
  else
  {
    m_scanner.moveTokenBackwards();
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
  string nameWithScope;
  do
  {
    checkToken(TOKEN_IDEN);

    nameWithScope = getLastToken().getLexeme();
    nameWithScope += "@";
    nameWithScope += m_semanticChecker.getCurrentScope();
    m_currentSymbols.push_back(
        make_pair(nameWithScope, SymbolData_t()));

    advanceToken();
    if (m_currentToken.getLexeme().compare("[") == 0)
    {
      m_scanner.moveTokenBackwards();
      dimension();
      advanceToken();
    }

#ifdef DEBUG
    cout << "::: current lexeme (line " << __LINE__ << "): " <<
        m_currentToken.getLexeme() << endl;
#endif
    if (m_currentToken.getLexeme().compare(",") != 0)
    {
      m_scanner.moveTokenBackwards();
    }
  } while (m_currentToken.getLexeme().compare(",") == 0);

  checkNativeDataType();

  setCurrentSymbolsData();

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
      string message = "se esperaba token: \"tipo_dato\", se recibio \"";
      message += TokenLexeme::getTokenString(m_currentToken.getToken());
      message += "\"";

      m_errorReporter->writeError(m_currentToken.getLine(),
          m_currentToken.getRow(), m_currentToken.getLexeme(), message);
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
  } while ((m_currentToken.getToken() == TOKEN_NEWLINE ||
            m_currentToken.getLexeme().compare(";") == 0));

  m_scanner.moveTokenBackwards();

#ifdef DEBUG
  cout << "::: stopped ignoring newlines()" << endl;
#endif
}

void Parser::advanceToken()
{
  m_currentToken = m_scanner.getNextTokenLexeme();
  ++m_nTokensProcessed;
}

TokenLexeme Parser::getLastToken()
{
  m_scanner.moveTokenBackwards();
  return m_scanner.getNextTokenLexeme();
}

bool Parser::isNativeDataType(const string& lexeme)
{
    if (lexeme.compare("caracter") == 0 ||
        lexeme.compare("entero") == 0   ||
        lexeme.compare("real") == 0     ||
        lexeme.compare("logico") == 0   ||
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


NativeType_t Parser::getLiteralType(TokenType_t token)
{
  NativeType_t type;
  switch (token)
  {
  case TOKEN_DEC :
  case TOKEN_HEX :
  case TOKEN_OCT :
    type = TYPE_INTEGER;
    break;
  case TOKEN_FLOAT :
    type = TYPE_FLOAT;
    break;
  case TOKEN_STRING :
    type = TYPE_STRING;
    break;
  case TOKEN_CHARCONST :
    type = TYPE_CHAR;
    break;
  case TOKEN_LOGICCONST :
    type = TYPE_BOOL;
    break;
  case TOKEN_INVALID :
  case TOKEN_IDEN :
  case TOKEN_KEYWORD :
  case TOKEN_ARITHOP :
  case TOKEN_LOGICOP :
  case TOKEN_RELOP :
  case TOKEN_DELIMITER :
  case TOKEN_LINECOMMENT :
  case TOKEN_MULTICOMMENT :
  case TOKEN_ASSIGNOP :
  case TOKEN_NEWLINE :
  default :
    cout << "error: invalid native type" << endl;
    break;
  }

  return type;
}

NativeType_t Parser::getTypeFromString(const string& typeString)
{
  NativeType_t type;

  switch (typeString.at(0))
  {
  case 'e' :
    type = TYPE_INTEGER;
    break;
  case 'r' :
    type = TYPE_FLOAT;
    break;
  case 'a' :
    type = TYPE_STRING;
    break;
  case 'c' :
    type = TYPE_CHAR;
    break;
  case 'l' :
    type = TYPE_BOOL;
    break;
  default :
    cout << "error: invalid native type" << endl;
    break;
  }

  return type;
}

void Parser::addSymbolAndReset(const string& name, SymbolData_t data)
{
  if (name.length() > 0)
  {
    m_semanticChecker.addSymbol(name, data);
  }

  m_currentSymbolName = "";
  m_currentSymbolData.type = TYPE_VOID;
  m_currentSymbolData.dimensions.clear();
  m_currentSymbolData.parametersList.clear();
  m_currentSymbolData.isFunction = false;
  m_currentSymbolData.isConstant = false;
  m_currentSymbolData.line = 1;
  m_currentSymbolData.scope = "";
}

void Parser::setCurrentSymbolsData()
{
  for (list<pair<string, SymbolData_t> >::iterator it =
       m_currentSymbols.begin(); it != m_currentSymbols.end(); ++it)
  {
    it->second.type = getTypeFromString(getLastToken().getLexeme());
    it->second.line = getLastToken().getLine();
    it->second.scope = m_semanticChecker.getCurrentScope();

    m_semanticChecker.addSymbol(it->first, it->second);
  }
}

