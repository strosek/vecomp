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

  m_validExpressions["sPs"] = 's';

  m_validExpressions["bRb"] = 'b';

  m_validExpressions["Mc"]  = 'c';
  m_validExpressions["Mf"]  = 'f';
  m_validExpressions["Mi"]  = 'i';
  m_validExpressions["Nb"]  = 'b';
}

NativeType_t SemanticChecker::getExpressionType()
{
  NativeType_t type = TYPE_VOID;

  if (!m_operations.empty())
  {
    type = SymbolData::getCharType(m_operations.top());
  }

  return type;
}

void SemanticChecker::pushVariableType(const string& name)
{
  if (m_symbolsTable.exists(name))
  {
#ifdef DEBUG
    cout << "pushing type: " <<
        SymbolData::getTypeChar(
        m_symbolsTable.getVariableType(name, m_controlStack.top())) << endl;
    pushOperand(m_symbolsTable.getVariableType(name, m_controlStack.top()));
#endif
  }
}

void SemanticChecker::pushFunctionType(const string& name,
                                      const string& parameters)
{
  pushOperand(m_symbolsTable.getFunctionType(name, parameters));
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
    if (expectedType != m_operations.top())
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
  while (!m_operations.empty())
  {
    cout << m_operations.top() << ", ";
    m_operations.pop();
  }
  cout << endl;
}

