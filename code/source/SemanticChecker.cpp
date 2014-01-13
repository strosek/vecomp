/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

using namespace std;

SemanticChecker::SemanticChecker(ErrorReporter * errorReporter) :
  m_errorReporter(errorReporter),
  m_symbolsTable(),
  m_isMainPresent(false),
  m_isReturnCalled(false),
  m_isReturnRequired(false),
  m_forLevel(0),
  m_switchLevel(0),
  m_validExpressions(),
  m_imports(),
  m_controlStack(),
  m_expressionTypes()
{
}

NativeType_t getExpressionType()
{
  NativeType_t type = TYPE_VOID;

  if (!m_operands.empty())
  {
    type = getCharType(m_operands.top());
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

bool SemanticChecker::isInFor() const
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

void SemanticChecker::checkExpressionType(NativeType_t expectedType)
{
}

void SemanticChecker::checkImported(const string& import)
{
  if (m_imports.find(import) == m_imports.end())
  {
    // TODO: Write error
  }
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

string SemanticChecker::getTypeChar(NativeType_t type)
{
  char typeChar = TYPECHAR_INVALID;

  switch (type)
  {
  case TYPE_VOID :
    typeChar = TYPECHAR_VOID;
    break;
  case TYPE_INTEGER:
    typeChar = TYPECHAR_INTEGER;
    break;
  case TYPE_CHAR:
    typeChar = TYPECHAR_CHAR;
    break;
  case TYPE_FLOAT :
    typeChar = TYPECHAR_CHAR;
    break;
  case TYPE_STRING :
    typeChar = TYPECHAR_STRING;
    break;
  case TYPE_BOOL :
    typeChar = TYPECHAR_BOOL;
    break;
  default :
#ifdef DEBUG
    cout << "error: invalid native type" << endl;
#endif
    break;
  }

  return typeChar;
}

string SemanticChecker::getCharType(char typeChar)
{
  NativeType_t type = TYPE_VOID;

  switch (typeChar)
  {
  case TYPECHAR_INTEGER :
    type = TYPE_INTEGER;
    break;
  case TYPECHAR_FLOAT :
    type = TYPE_FLOAT;
    break;
  case TYPECHAR_CHAR :
    type = TYPE_CHAR;
    break;
  case TYPECHAR_STRING :
    type = TYPE_STRING;
    break;
  case TYPECHAR_BOOL :
    type = TYPE_BOOL;
    break;
  case TYPECHAR_VOID :
    type = TYPE_VOID;
    break;
  case TYPECHAR_INVALID :
    type = TYPE_VOID;
    break;
  default :
#ifdef DEBUG
    COUT << "error: invalid char type" << endl;
#endif
  } 

  return type;
}

