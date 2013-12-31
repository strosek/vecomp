/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

using namespace std;

SemanticChecker::SemanticChecker(ErrorReporter * errorReporter) :
  m_isMainPresent(false),
  m_isInFor(false),
  m_isInSwitch(false),
  m_isReturnCalled(false),
  m_errorReporter(errorReporter),
  m_expressionTypes(),
  m_controlStack(),
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

  if (m_symbolsTable.find(token.getLexeme()) == m_symbolsTable.end())
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
  if (m_symbolsTable.find(token.getLexeme()) != m_symbolsTable.end())
  {
    if (parametersTypes.size() == m_symbolsTable[token.getLexeme()].
            parametersTypes.size())
    {
      list<string>& tableValues =
          m_symbolsTable[token.getLexeme()].parametersTypes;

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
        message = "funcion no declarada con esos parametros";
      }
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

string SemanticChecker::getFunctionType(const string& iden,
                                        const list<string>& parametersTypes)
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
  return m_isInFor;
}

bool SemanticChecker::isMainPresent()
{
  return m_isMainPresent;
}

bool SemanticChecker::isInSwitch()
{
  return m_isInSwitch;
}

void SemanticChecker::setMainPresent(bool isPresent)
{
}

void SemanticChecker::setInFor(bool inFor)
{
}

void SemanticChecker::setInSwitch(bool inSwitch)
{
}

void SemanticChecker::enterToScope(const string& scope)
{
}

void SemanticChecker::exitCurrentScope()
{
}

bool SemanticChecker::isSymbolPresent(const string& name)
{
  return false;
}

bool SemanticChecker::parametersMatch(const string& name,
    const list<string>& parametersTypes)
{
  return false;
}

