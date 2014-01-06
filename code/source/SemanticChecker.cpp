/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

SemanticChecker::SemanticChecker(ErrorReporter * errorReporter) :
  m_isMainPresent(false),
  m_forLevel(0),
  m_switchLevel(0),
  m_isReturnCalled(false),
  m_isReturnRequired(false),
  m_errorReporter(errorReporter),
  m_expressionTypes(),
  m_controlStack(),
  m_imports(),
  m_symbolsTable(),
  m_typesStack()
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
   * U = Unary (-)
   *
   * VALUES
   *
   * r = real 
   * i = integer
   * b = boolean
   * s = string
   */
  m_expressionTypes["iSi"] = 'i';
  m_expressionTypes["iAi"] = 'i';
  m_expressionTypes["iPi"] = 'r';
  m_expressionTypes["iMi"] = 'i';
  m_expressionTypes["iRi"] = 'b';
  m_expressionTypes["iEi"] = 'b';

  m_expressionTypes["rSr"] = 'r';
  m_expressionTypes["rAr"] = 'r';
  m_expressionTypes["rPr"] = 'r';
  m_expressionTypes["rRr"] = 'b';
  m_expressionTypes["rEr"] = 'b';

  m_expressionTypes["rSi"] = 'r';
  m_expressionTypes["rAi"] = 'r';
  m_expressionTypes["rPi"] = 'r';
  m_expressionTypes["rRi"] = 'b';
  m_expressionTypes["rEi"] = 'b';
  m_expressionTypes["iSr"] = 'r';
  m_expressionTypes["iAr"] = 'r';
  m_expressionTypes["iPr"] = 'r';
  m_expressionTypes["iRr"] = 'b';
  m_expressionTypes["iEr"] = 'b';

  m_expressionTypes["sRs"] = 'b';
  m_expressionTypes["sSs"] = 's';
  m_expressionTypes["bLb"] = 'b';
  m_expressionTypes["bEb"] = 'b';

  m_expressionTypes["Ui"] = 'i';
  m_expressionTypes["Ur"] = 'r';
}

void SemanticChecker::checkVariableDeclared(const TokenLexeme& token)
{

  if (!isSymbolPresent(appendCurrentScope(token.getLexeme())) &&
      !isSymbolPresent(appendGlobalScope(token.getLexeme())))
  {
    m_errorReporter->writeError(token.getLine(), token.getRow(),
                                token.getLexeme(), "variable no declarada");
  }
}

void SemanticChecker::checkVariableNotDeclared(const TokenLexeme& token)
{
  if (isSymbolPresent(appendCurrentScope(token.getLexeme())))
  {
    string message = "variable ya declarada en este alcance con tipo: ";
    message += getTypeString(
        m_symbolsTable[appendCurrentScope(token.getLexeme())].type);

    m_errorReporter->writeError(token.getLine(), token.getRow(),
        token.getLexeme(), message);
  }
}

void SemanticChecker::checkFunctionDeclared(const TokenLexeme& token,
                                            ParametersList_t parametersList)
{
  bool isErrorFound = false;
  string message;
  if (isSymbolPresent(token.getLexeme()))
  {
    if (!parametersMatch(token.getLexeme(), parametersList))
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

void SemanticChecker::checkFunctionNotDeclared(const TokenLexeme& token,
                                               ParametersList_t parametersList)
{
  string message;
  if (isSymbolPresent(token.getLexeme()))
  {
    if (parametersMatch(token.getLexeme(), parametersList))
    {
      message = "funcion ya declarada con esos parametros";

      m_errorReporter->writeError(token.getLine(), token.getRow(),
                                  token.getLexeme(), message);
    }
  }
}

void SemanticChecker::checkModifiable(const TokenLexeme& token)
{
  if (isSymbolPresent(token.getLexeme()))
  {
    if (m_symbolsTable[token.getLexeme()].isConstant)
    {
      m_errorReporter->writeError(token.getLine(), token.getRow(),
          token.getLexeme(), "no se puede modificar constante");
    }
  }
}

void SemanticChecker::checkDimensions(TokenLexeme& token, vector<int> sizes)
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

void SemanticChecker::checkImported(const std::string& package, int line,
                                    int row)
{
  if (m_imports.find(package) == m_imports.end())
  {
    m_errorReporter->writeError(line, row, package, "libreria no importada");
  }
}

string SemanticChecker::getFunctionType(const string& iden,
                                        ParametersList_t parametersList)
{
  string returnValue;

  if (isSymbolPresent(iden)) 
  {
    if (parametersMatch(iden, parametersList))
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

bool SemanticChecker::isReturnCalled()
{
  return m_isReturnCalled;
}

void SemanticChecker::setMainPresent(bool isPresent)
{
  m_isMainPresent = isPresent;
}

void SemanticChecker::setReturnRequired(bool isRequired)
{
  m_isReturnRequired = isRequired;
}

void SemanticChecker::setReturnCalled(bool isCalled)
{
  m_isReturnCalled = isCalled;
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
  --m_forLevel;
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
                                      ParametersList_t parametersList)
{
  if (isSymbolPresent(name))
  {
    if (parametersList.size() == m_symbolsTable[name].parametersList.size())
    {
      string parametersInTable =
          getParametersString(m_symbolsTable[name].parametersList);
      string actualParameters = getParametersString(parametersList);

      if (parametersInTable.compare(actualParameters) != 0)
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

void SemanticChecker::printSymbolsTable()
{
  const int k_fieldWidthLong = 30;
  const int k_fieldWidthShort = 9;

  cout << setw(k_fieldWidthLong) << "NAME" << 
      setw(k_fieldWidthLong) << "TYPE" <<
      setw(k_fieldWidthShort) << "DIM" << 
      setw(k_fieldWidthShort) << "FUNCTION" <<
      setw(k_fieldWidthLong) << "PARAMS" << 
      setw(k_fieldWidthLong) << "SCOPE" <<
      setw(k_fieldWidthShort) << "CONST" << 
      setw(k_fieldWidthShort) << "LINE" << endl;
  cout << endl;

  for (map<string, SymbolData_t>::iterator it = m_symbolsTable.begin();
       it != m_symbolsTable.end(); ++it)
  {
    cout << setw(k_fieldWidthLong) << it->first;
    cout << setw(k_fieldWidthLong) << getTypeString(it->second.type);
    cout << setw(k_fieldWidthShort) << it->second.dimensions.size();
    cout << setw(k_fieldWidthShort) << boolalpha << it->second.isFunction;
    if (!it->second.parametersList.empty())
    {
      cout << setw(k_fieldWidthLong) <<
          getParametersString(it->second.parametersList); 
    }
    else
    {
      cout << setw(k_fieldWidthLong) << "nada";
    }
    cout << setw(k_fieldWidthLong) << it->second.scope;
    cout << setw(k_fieldWidthShort) << boolalpha << it->second.isConstant;
    cout << setw(k_fieldWidthShort) << it->second.line;
    cout << endl;
  }
}

string SemanticChecker::getTypeString(NativeType_t type)
{
  string typeString;

  switch (type)
  {
    case TYPE_INTEGER :
      typeString = "entero";
      break;
    case TYPE_CHAR:
      typeString = "caracter";
      break;
    case TYPE_FLOAT :
      typeString = "real";
      break;
    case TYPE_STRING :
      typeString = "alfabetico";
      break;
    case TYPE_BOOL :
      typeString = "logico";
      break;
    case TYPE_VOID :
      typeString = "nada";
      break;
    default :
#ifdef DEBUG
      cout << "error: unkown type" << endl;
#endif
      break;
  }

  return typeString;
}

string SemanticChecker::getParametersString(ParametersList_t parametersList)
{
  stringstream stringBuilder;
  for (ParametersList_t::iterator it = parametersList.begin();
       it != parametersList.end(); ++it)
  {
    stringBuilder << it->first << getTypeString(it->second);
  }

  return stringBuilder.str();
}

void SemanticChecker::addSymbol(const string& name, SymbolData_t data)
{
  if (!isSymbolPresent(name))
  {
    m_symbolsTable[name] = data;
  }
}

void SemanticChecker::addSymbols(
    list<pair<string, SymbolData_t> > symbols)
{
  for (list<pair<string, SymbolData_t> >::iterator it = symbols.begin();
       it != symbols.end(); ++it)
  {
    addSymbol(it->first, it->second);
  }
}

void SemanticChecker::checkReturnShouldBeCalled(int functionEndLine)
{
  if (m_isReturnRequired && !m_isReturnCalled)
  {
    m_errorReporter->writeError(functionEndLine, 1, "",
        "funcion con tipo no puede omitir regresa");
  }
  else if (!m_isReturnRequired && m_isReturnCalled)
  {
    m_errorReporter->writeError(functionEndLine, 1, "",
        "funcion sin tipo no puede regresar un valor");
  }
  
  m_isReturnRequired = false;
  m_isReturnCalled = false;
}

string SemanticChecker::appendCurrentScope(string name)
{
  name += "@";
  name += getCurrentScope();

  return name;
}

string SemanticChecker::appendGlobalScope(string name)
{
  name += "@global";

  return name;
}

void SemanticChecker::pushOperand(TokenLexeme& token)
{
  string symbol = getActualSymbol(token.getLexeme(), false);

  if (isSymbolPresent(symbol))
  {
    m_typesStack.push(getTypeChar(m_symbolsTable[symbol].type));

    checkExpression(token.getLexeme(), token.getLine(), token.getRow());
  }
}

void SemanticChecker::pushOperand(char operand, int line, int row,
                                  const string& lexeme)
{
  m_typesStack.push(operand);

  checkExpression(lexeme, line, row);
}

void SemanticChecker::pushOperator(char op, int line, int row, 
                                   const string& lexeme)
{
  m_typesStack.push(op);

  checkExpression(lexeme, line, row);
}

string SemanticChecker::getActualSymbol(string iden, bool isFunction)
{
  if (isFunction)
  {
    iden += "()";
  }

  string symbolName;

  symbolName = appendCurrentScope(iden);
  if (!isSymbolPresent(symbolName))
  {
    symbolName = appendGlobalScope(iden);
    if (!isSymbolPresent(symbolName))
    {
      symbolName = iden;
    }
  }

  return symbolName;
}

char SemanticChecker::getTypeChar(NativeType_t type)
{
  char typeChar = 'n';

  switch (type)
  {
  case TYPE_INTEGER :
  case TYPE_CHAR :
    typeChar = 'i';
    break;
  case TYPE_FLOAT :
    typeChar = 'r';
    break;
  case TYPE_STRING :
    typeChar = 's';
    break;
  case TYPE_BOOL :
    typeChar = 'b';
    break;
  case TYPE_VOID :
    typeChar = 'v';
    break;
  default :
#ifdef DEBUG
    cout << "error: invalid type" << endl;
#endif
    break;
  }

  return typeChar;
}

void SemanticChecker::checkExpression(const string& lexeme, int line, int row)
{
  string expression = "";
  bool isStackComplete = false;
  const size_t k_maxExpressionElements = 3;
  if (m_typesStack.size() == k_maxExpressionElements)
  {
    expression += m_typesStack.top();
    m_typesStack.pop();
    expression += m_typesStack.top();
    m_typesStack.pop();
    expression += m_typesStack.top();
    m_typesStack.pop();

    isStackComplete = true;
  }
  else if (m_typesStack.top() == 'U')
  {
    expression += m_typesStack.top();
    m_typesStack.pop();
    expression += m_typesStack.top();
    m_typesStack.pop();

    isStackComplete = true;
  }

  if (isStackComplete)
  {
#ifdef DEBUG
    cout << "::: types in expression: " << expression << endl;
#endif

    if (m_expressionTypes.find(expression) != m_expressionTypes.end())
    {
      m_typesStack.push(m_expressionTypes[expression]);
    }
    else
    {
      m_typesStack.push('n');

      m_errorReporter->writeError(line, row, lexeme,
          "tipo invalido en expresion");
    }
  }
}

void SemanticChecker::checkExpressionType(char expectedType, 
                                          const TokenLexeme& token)
{
  if (m_typesStack.top() != expectedType)
  {
    m_errorReporter->writeError(token.getLine(), token.getRow(), token.getLexeme(), 
        "expresion resulta en tipo invalido");
  }

  clearTypesStack();
}

NativeType_t SemanticChecker::getExpressionType()
{
  NativeType_t type = TYPE_VOID;
  switch (m_typesStack.top())
  {
  case 'i' :
    type = TYPE_INTEGER;
    break;
  case 'r' :
    type = TYPE_FLOAT;
    break;
  case 's' :
    type = TYPE_STRING;
    break;
  case 'b' :
    type = TYPE_BOOL;
    break;
  default :
#ifdef DEBUG
    cout << "error: unknown type" << endl;
#endif
    break;
  }

  clearTypesStack();

  return type;
}

void SemanticChecker::clearTypesStack()
{
  while (!m_typesStack.empty())
  {
    m_typesStack.pop();
  }
}

ParametersList_t SemanticChecker::getParametersListFromString(
    const string typesString)
{
  int i, f, a, s, b;
  i = f = a = s = b = 0;
  ParametersList_t parametersList;

  for (size_t i = 0; i < typesString.size(); ++i)
  {
    switch (typesString.at(i))
    {
    case 'i' :
      ++i;
      break;
    case 'f' :
      ++i;
      break;
    case 'a' :
      ++i;
      break;
    case 's' :
      ++i;
      break;
    case 'b' :
      ++b;
      break;
    default :
#ifdef DEBUG
      cout << "error: unknown type" << endl;
#endif
      break;
    }

    if (i > 0)
    {
      parametersList.push_back(make_pair(i, TYPE_INTEGER));
    }
    if (f > 0)
    {
      parametersList.push_back(make_pair(f, TYPE_INTEGER));
    }
    if (a > 0)
    {
      parametersList.push_back(make_pair(a, TYPE_INTEGER));
    }
    if (s > 0)
    {
      parametersList.push_back(make_pair(s, TYPE_INTEGER));
    }
    if (b > 0)
    {
      parametersList.push_back(make_pair(b, TYPE_INTEGER));
    }
  }

  return parametersList;
}

