/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

#ifdef DEBUG
# include <iostream>
#endif
#include <sstream>

using namespace std;

SemanticChecker::SemanticChecker() :
  m_errorReporter(),
  m_symbolsTable(),
  m_isMainPresent(false),
  m_isReturnCalled(false),
  m_isReturnRequired(false),
  m_forLevel(0),
  m_switchLevel(0),
  m_validExpressions(),
  m_imports(),
  m_controlStack(),
  m_operations()
{
  m_validExpressions["cPc"] = 'c';
  m_validExpressions["cPf"] = 'f';
  m_validExpressions["cPi"] = 'i';
  m_validExpressions["fPc"] = 'f';
  m_validExpressions["fPf"] = 'f';
  m_validExpressions["fPi"] = 'f';
  m_validExpressions["iPc"] = 'i';
  m_validExpressions["iPf"] = 'f';
  m_validExpressions["iPi"] = 'i';

  m_validExpressions["cMc"] = 'c';
  m_validExpressions["cMf"] = 'f';
  m_validExpressions["cMi"] = 'i';
  m_validExpressions["fMc"] = 'f';
  m_validExpressions["fMf"] = 'f';
  m_validExpressions["fMi"] = 'f';
  m_validExpressions["iMc"] = 'i';
  m_validExpressions["iMf"] = 'f';
  m_validExpressions["iMi"] = 'i';

  m_validExpressions["cAc"] = 'c';
  m_validExpressions["cAf"] = 'f';
  m_validExpressions["cAi"] = 'i';
  m_validExpressions["fAc"] = 'f';
  m_validExpressions["fAf"] = 'f';
  m_validExpressions["fAi"] = 'f';
  m_validExpressions["iAc"] = 'i';
  m_validExpressions["iAf"] = 'f';
  m_validExpressions["iAi"] = 'i';

  m_validExpressions["cRc"] = 'c';
  m_validExpressions["cRf"] = 'f';
  m_validExpressions["cRi"] = 'i';
  m_validExpressions["fRc"] = 'f';
  m_validExpressions["fRf"] = 'f';
  m_validExpressions["fRi"] = 'f';
  m_validExpressions["iRc"] = 'i';
  m_validExpressions["iRf"] = 'f';
  m_validExpressions["iRi"] = 'i';

  m_validExpressions["sPs"] = 's';

  m_validExpressions["bLb"] = 'b';

  m_validExpressions["Mc"]  = 'c';
  m_validExpressions["Mf"]  = 'f';
  m_validExpressions["Mi"]  = 'i';

  m_validExpressions["Nb"]  = 'b';
}

NativeType_t SemanticChecker::getExpressionType()
{
  NativeType_t type = TYPE_INVALID;

  if (!m_operations.empty())
  {
    type = SymbolData::getCharType(m_operations.top());
    m_operations.pop();
  }

  return type;
}

string SemanticChecker::getCurrentArguments(unsigned int nParameters)
{
  string arguments;
  for (unsigned int i = 0; i < nParameters; ++i)
  {
    if (!m_operations.empty())
    {
      arguments.insert(arguments.begin(), m_operations.top());
      m_operations.pop();
    }
  }

  return arguments;
}

void SemanticChecker::pushVariableType(const string& name)
{
  if (m_symbolsTable.exists(name))
  {
#ifdef DEBUG
    cout << "push variable: " << name << ", type: " << 
        SymbolData::getTypeChar(
        m_symbolsTable.getVariableType(name, m_controlStack.top())) << endl;
#endif
    pushOperand(SymbolData::getTypeChar(
        m_symbolsTable.getVariableType(name, m_controlStack.top())));
  }
  else
  {
#ifdef DEBUG
    cout << "variable not found: " << name << endl;
#endif
  }
}

void SemanticChecker::pushFunctionType(const string& name,
                                       const string& parameters)
{
  pushOperand(SymbolData::getTypeChar(
      m_symbolsTable.getFunctionType(name, parameters)));
}

bool SemanticChecker::isInFor() const
{
  if (m_forLevel > 0)
  {
    return true;
  }

  return false;
}

bool SemanticChecker::isInSwitch() const
{
  if (m_switchLevel > 0)
  {
    return true;
  }

  return false;
}

bool SemanticChecker::isMainPresent() const
{
  return m_isMainPresent;
}

void SemanticChecker::addImport(const string& import)
{
  m_imports.insert(import);
}

void SemanticChecker::declare(SymbolData& data)
{
  if (data.isFunction())
  {
    string scope = "global";
    data.setScope(scope);
    m_symbolsTable.checkFunctionDeclarable(data.getName(), 
                                           data.getParameters());
  }
  else
  {
    data.setScope(m_controlStack.top());
    m_symbolsTable.checkDeclarable(data.getName(), data.getScope());
  }

  m_symbolsTable.insert(data.getName(), data);
}

void SemanticChecker::checkExpressionType(NativeType_t expectedType)
{
  if (!m_operations.empty())
  {
    if (SymbolData::getTypeChar(expectedType) != m_operations.top())
    {
      string message = "tipo de expresion no esperado, se esperaba: \"";
      message += SymbolData::getTypeString(expectedType);
      message += "\"";
      m_errorReporter->writeErrorWithPosition(message);
    }
    m_operations.pop();
  }
}

void SemanticChecker::checkExpressionType(NativeType_t expectedType,
                                          const string& errorMessage)
{
  if (!m_operations.empty())
  {
    if (SymbolData::getTypeChar(expectedType) != m_operations.top())
    {
      m_errorReporter->writeErrorWithPosition(errorMessage);
    }
    m_operations.pop();
  }
}

void SemanticChecker::checkAssignType(const string& variable)
{
  NativeType_t varType = m_symbolsTable.getVariableType(variable,
                                                        m_controlStack.top());

  if (!m_operations.empty())
  {
    if (varType != SymbolData::getCharType(m_operations.top()))
    {
      string message = "tipo invalido en asignacion, se esperaba: ";
      message += SymbolData::getTypeString(varType);
      m_errorReporter->writeErrorWithPosition(message);
    }
    m_operations.pop();
  }
}

void SemanticChecker::checkReturnType(NativeType_t type)
{
  if (!m_operations.empty())
  {
    if (type != SymbolData::getCharType(m_operations.top()))
    {
      string message = "tipo invalido en regresa, se esperaba: ";
      message += SymbolData::getTypeString(type);
      m_errorReporter->writeErrorWithPosition(message);
    }
    m_operations.pop();
  }
}

void SemanticChecker::checkDimensionsMatch(const string& variable,
                                           unsigned int dimensions)
{
  unsigned int declaredDimensions = m_symbolsTable.getDimensions(
      variable, m_controlStack.top());
#ifdef DEBUG
    cout << "given dimensions: " << dimensions << "declared dimensions: " <<
        declaredDimensions << endl;
#endif

  ostringstream messageBuilder;
  if (declaredDimensions > dimensions)
  {
    messageBuilder << "faltan dimensiones, variable declarada con: " <<
      declaredDimensions;
    messageBuilder.flush();
    m_errorReporter->writeErrorWithPosition(messageBuilder.str());
  }
  else if (declaredDimensions < dimensions)
  {
    messageBuilder << "exceso de dimensiones, variable declarada con: " <<
      declaredDimensions;
    messageBuilder.flush();
    m_errorReporter->writeErrorWithPosition(messageBuilder.str());
  }
}

void SemanticChecker::checkModifiable(const std::string& name)
{
  m_symbolsTable.checkModifiable(name, m_controlStack.top());
}

void SemanticChecker::checkDeclared(const string& name)
{
  m_symbolsTable.checkDeclared(name, m_controlStack.top());
}

void SemanticChecker::checkDeclared(const string& name,
                                    const string& parameters)
{
  m_symbolsTable.checkFunctionDeclared(name, parameters);
}

void SemanticChecker::checkImported(const string& import)
{
  if (m_imports.find(import) == m_imports.end())
  {
    string message = "no se ha importado la libreria: \"";
    message += import;
    message += "\"";
    m_errorReporter->writeErrorWithPosition(message);
  }
}

void SemanticChecker::checkReturnRequired()
{
  if (m_isReturnRequired && !m_isReturnCalled)
  {
    m_errorReporter->writeErrorWithPosition(
        "funcion con tipo no llama a regresa");
  }
  else if (!m_isReturnRequired && m_isReturnCalled)
  {
    m_errorReporter->writeErrorWithPosition(
        "funcion sin tipo llama a regresa con tipo");
  }

  m_isReturnCalled = false;
  m_isReturnRequired = false;
}

void SemanticChecker::enterFor()
{
  ++m_forLevel;
}

void SemanticChecker::enterSwitch()
{
  ++m_switchLevel;
}

void SemanticChecker::enterToScope(const string& scope)
{
  m_controlStack.push(scope);
}

void SemanticChecker::exitCurrentScope()
{
  m_controlStack.pop();
}

void SemanticChecker::exitFor()
{
  --m_forLevel;
}

void SemanticChecker::exitSwitch()
{
  --m_switchLevel;
}

void SemanticChecker::evaluateUnaryExpression()
{
  string expression;

  const int MAX_OPERANDS = 2;
  if (m_operations.size() >= MAX_OPERANDS)
  {
    int i = 0;
    while (i < MAX_OPERANDS && !m_operations.empty())
    {
      expression.insert(expression.begin(), m_operations.top());
      m_operations.pop();

      ++i;
    }
#ifdef DEBUG
    cout << "evaluating unary: " << expression << endl;
#endif

    if (m_validExpressions.find(expression) != m_validExpressions.end())
    {
      m_operations.push(m_validExpressions[expression]);
    }
    else
    {
      if (!expression.empty())
      {
        m_operations.push(TYPECHAR_INVALID);
      }

      if (expression.size() >= 2)
      {
        ostringstream messageBuilder;
        messageBuilder << "conflicto de tipos en operacion: " <<
            expression.at(0) <<
            SymbolData::getTypeString(
            SymbolData::getCharType(expression.at(1)));
        m_errorReporter->writeErrorWithPosition(messageBuilder.str());
      }
      else
      {
        m_errorReporter->writeErrorWithPosition(
            "conflicto en tipos en operacion");
      }
    }
  }
}

void SemanticChecker::evaluateBinaryExpression()
{
  string expression;

  const int MAX_OPERANDS = 3;
  if (m_operations.size() >= MAX_OPERANDS)
  {
    int i = 0;
    while (i < MAX_OPERANDS && !m_operations.empty())
    {
      expression.insert(expression.begin(), m_operations.top());
      m_operations.pop();

      ++i;
    }
#ifdef DEBUG
    cout << "evaluating binary: " << expression << endl;
#endif

    if (m_validExpressions.find(expression) != m_validExpressions.end())
    {
      m_operations.push(m_validExpressions[expression]);
    }
    else
    {
      if (!expression.empty())
      {
        m_operations.push(TYPECHAR_INVALID);
      }

      if (expression.size() >= 2)
      {
        ostringstream messageBuilder;
        messageBuilder << "conflicto de tipos en operacion: " <<
            SymbolData::getTypeString(
            SymbolData::getCharType(expression.at(0))) << expression.at(1) <<
            SymbolData::getTypeString(
            SymbolData::getCharType(expression.at(2)));
        m_errorReporter->writeErrorWithPosition(messageBuilder.str());
      }
      else
      {
        m_errorReporter->writeErrorWithPosition(
            "conflicto en tipos en operacion");
      }
    }
  }
}

void SemanticChecker::pushOperand(char op)
{
  m_operations.push(op);
}

void SemanticChecker::pushOperator(char op)
{
  m_operations.push(op);
}

void SemanticChecker::setMainPresent(bool isPresent)
{
  m_isMainPresent = isPresent;
}

void SemanticChecker::setReturnCalled(bool isCalled)
{
  m_isReturnCalled = isCalled;
}

void SemanticChecker::setReturnRequired(bool isRequired)
{
  m_isReturnRequired = isRequired;
}

void SemanticChecker::printSymbolsTable() const
{
  cout << m_symbolsTable;
}

void SemanticChecker::setErrorReporter(ErrorReporter* errorReporter)
{
  m_errorReporter = errorReporter;
  m_symbolsTable.setErrorReporter(errorReporter);
}

void SemanticChecker::printTypesStack()
{
  cout << "::: Types stack :::::::::::::::::::::::::::::::::::::::::::" << endl;
  cout << "::: Size: " << m_operations.size() << endl;

  string invertedStack;
  while (!m_operations.empty())
  {
    invertedStack.insert(invertedStack.begin(), m_operations.top());
    m_operations.pop();
  }

  for (string::iterator it = invertedStack.begin();
       it != invertedStack.end(); ++it)
  {
    m_operations.push(*it);
  }

  cout << "::: Values: " << invertedStack << endl;
}

