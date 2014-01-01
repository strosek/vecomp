/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

#include <utility>

using namespace std;

SemanticChecker::SemanticChecker(ErrorReporter * errorReporter) :
  m_isMainPresent(false),
  m_forLevel(0),
  m_switchLevel(0),
  m_isReturnCalled(false),
  m_errorReporter(errorReporter),
  m_expressionTypes(),
  m_controlStack(),
  m_imports(),
  m_symbolsTable()
{
  /* OPERATORS
   *
   * S = Sum (+)
   * A = Arithmetic (- * /)
   * P = Power (^)
   * M = Modulo (%)
   * R = Relational (< > <= >=)
   * E = Equality (!= ==)
   * L = Logical (&& ||)
   *
   * VALUES
   *
   * f = float
   * i = integer
   * b = boolean
   * s = string
   */
  m_expressionTypes["iSi"] = "i";
  m_expressionTypes["iAi"] = "i";
  m_expressionTypes["iPi"] = "f";
  m_expressionTypes["iMi"] = "i";
  m_expressionTypes["iRi"] = "b";
  m_expressionTypes["iEi"] = "b";

  m_expressionTypes["fSf"] = "f";
  m_expressionTypes["fAf"] = "f";
  m_expressionTypes["fPf"] = "f";
  m_expressionTypes["fRf"] = "b";
  m_expressionTypes["fEf"] = "b";

  m_expressionTypes["fSi"] = "f";
  m_expressionTypes["fAi"] = "f";
  m_expressionTypes["fPi"] = "f";
  m_expressionTypes["fRi"] = "b";
  m_expressionTypes["fEi"] = "b";
  m_expressionTypes["iSf"] = "f";
  m_expressionTypes["iAf"] = "f";
  m_expressionTypes["iPf"] = "f";
  m_expressionTypes["iRf"] = "b";
  m_expressionTypes["iEf"] = "b";

  m_expressionTypes["sRs"] = "b";
  m_expressionTypes["s+s"] = "s";
  m_expressionTypes["bLb"] = "b";
  m_expressionTypes["bEb"] = "b";
}

void SemanticChecker::checkVariableDeclared(const TokenLexeme& token,
    const string& type, const string& scope)
{
  string message;
  bool isErrorFound = false;

  if (!isSymbolPresent(token.getLexeme()))
  {
    message = "variable no declarada";
    isErrorFound = true;
  }
  else if (m_symbolsTable[token.getLexeme()].type.compare(type) != 0)
  {
    message = "variable declarada con diferente tipo";
    isErrorFound = true;
  }
  else if (m_symbolsTable[token.getLexeme()].scope.compare(scope) != 0)
  {
    message = "variable no declarada en este alcance";
    isErrorFound = true;
  }

  if (isErrorFound)
  {
    m_errorReporter->writeError(token.getLine(), token.getRow(),
                                token.getLexeme(), message);
  }
}

void SemanticChecker::checkFunctionDeclared(const TokenLexeme& token,
                                            list<string> parametersTypes)
{
  bool isErrorFound = false;
  string message;
  if (isSymbolPresent(token.getLexeme()))
  {
    if (!parametersMatch(token.getLexeme(), parametersTypes))
    {
      isErrorFound = true;
      message = "funcion no declarada con esos parametros";
    }
  }
  else
  {
    isErrorFound = true;
    message = "funcion no declarada";
  }

  if (isErrorFound)
  {
    m_errorReporter->writeError(token.getLine(), token.getRow(),
                                token.getLexeme(), message);
  }
}

void SemanticChecker::checkModifiable(const TokenLexeme& token)
{
  if (m_symbolsTable.find(token.getLexeme()) != m_symbolsTable.end())
  {
    if (!m_symbolsTable[token.getLexeme()].isConstant)
    {
      m_errorReporter->writeError(token.getLine(), token.getRow(),
                                  token.getLexeme(), "variable es constante");
    }
  }
}

void SemanticChecker::checkExpression(const string& expression,
                                      int line, int row)
{
  if (m_expressionTypes.find(expression) == m_expressionTypes.end())
  {
    m_errorReporter->writeError(line, row, " ",
        "expresion con operandos de tipo invalido");
  }
}

void SemanticChecker::checkDimensions(TokenLexeme& token, 
                                      std::vector<int> sizes)
{
  if (isSymbolPresent(token.getLexeme()))
  {
    string message;

    if (m_symbolsTable[token.getLexeme()].dimensions.size() <= 0)
    {
      message = "variable no dimensionada";
    }
    else if (sizes.size() < m_symbolsTable[token.getLexeme()].dimensions.size())
    {
      m_errorReporter->writeError(token.getLine(), token.getRow(),
          token.getLexeme(), "fatan dimensiones");
    }
    else if (sizes.size() > m_symbolsTable[token.getLexeme()].dimensions.size())
    {
      m_errorReporter->writeError(token.getLine(), token.getRow(),
          token.getLexeme(), "exceso de dimensiones");
    }
  }
}

string SemanticChecker::getFunctionType(const string& iden,
                                        list<string>& parametersTypes)
{
  string returnValue;

  if (isSymbolPresent(iden)) 
  {
    if (parametersMatch(iden, parametersTypes))
    {
      returnValue = m_symbolsTable[iden].type;
    }
  }

  return returnValue;
}

string SemanticChecker::getCurrentScope()
{
  return m_controlStack.top();
}

bool SemanticChecker::isInFor()
{
  if (m_forLevel > 0)
  {
    return true;
  }

  return false;
}

bool SemanticChecker::isMainPresent()
{
  return m_isMainPresent;
}

bool SemanticChecker::isInSwitch()
{
  if (m_switchLevel > 0)
  {
    return true;
  }

  return false;
}

void SemanticChecker::setMainPresent(bool isPresent)
{
  m_isMainPresent = isPresent;
}

void SemanticChecker::enterFor()
{
  ++m_forLevel;
}

void SemanticChecker::enterSwitch()
{
  ++m_switchLevel;
}

void SemanticChecker::exitFor()
{
  --m_switchLevel;
}

void SemanticChecker::exitSwitch()
{
  --m_switchLevel;
}

void SemanticChecker::enterToScope(const string& scope)
{
  m_controlStack.push(scope);
}

void SemanticChecker::exitCurrentScope()
{
  m_controlStack.pop();
}

bool SemanticChecker::isSymbolPresent(const string& name)
{
  if (m_symbolsTable.find(name) != m_symbolsTable.end())
  {
    return true;
  }

  return false;
}

bool SemanticChecker::parametersMatch(const string& name,
                                      list<string>& parametersTypes)
{
  if (m_symbolsTable.find(name) != m_symbolsTable.end())
  {
    bool isErrorFound = false;
    if (parametersTypes.size() == m_symbolsTable[name].parametersTypes.size())
    {
      list<string>& tableValues =
          m_symbolsTable[name].parametersTypes;

      for (list<string>::iterator tableIt = tableValues.begin(),
           checkIt = parametersTypes.begin();

           tableIt != tableValues.end() && checkIt != parametersTypes.end() &&
           !isErrorFound;

           ++tableIt, ++checkIt)
      {
        if (tableIt->compare((*checkIt)) != 0)
        {
          isErrorFound = true;
        }
      }

      if (isErrorFound)
      {
        return false;
      }
    }
  }

  return true;
}

void SemanticChecker::addImport(const TokenLexeme& import)
{
  if (m_imports.insert(import.getLexeme()).second != true)
  {
    m_errorReporter->writeError(import.getLine(), import.getRow(),
        import.getLexeme(), "libreria importada repetida");
  }
}

