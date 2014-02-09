/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

#ifdef DEBUG
# include <iostream>
#endif

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
  m_operands(),
  m_operators()
{
}

NativeType_t SemanticChecker::getExpressionType()
{
  NativeType_t type = TYPE_VOID;

  if (!m_operands.empty())
  {
    type = SymbolData::getCharType(m_operands.top());
  }

  return type;
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
  if (!m_operands.empty())
  {
    if (expectedType != m_operands.top())
    {
      string message = "tipo de expresion no esperado, se esperaba: \"";
      message += SymbolData::getTypeString(expectedType);
      message += "\"";
      m_errorReporter->writeErrorWithPosition(message);
    }
  }
}

void SemanticChecker::checkDeclared(const SymbolData& data)
{
  if (data.isFunction())
  {
    m_symbolsTable.checkFunctionDeclared(data.getName(), data.getParameters());
  }
  else
  {
    m_symbolsTable.checkDeclared(data.getName(), data.getScope(),
                                 data.getDimensions(), data.getType());
  }
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

void SemanticChecker::pushOperand(char op)
{
  m_operators.push(op);
}

void SemanticChecker::pushOperator(char op)
{
  m_operands.push(op);
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

