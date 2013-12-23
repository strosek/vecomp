/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

using namespace std;

SemanticChecker::SemanticChecker(ErrorReporter * errorReporter) :
  m_isMainPresent(false),
  m_isInFor(false),
  m_isInSwitch(false),
  m_isReturnCalled(false),
  m_errorReporter(),
  m_expressionTypes(),
  m_controlStack(),
  m_symbolsTable()
{
}

void SemanticChecker::checkVariableDeclared(const string& iden)
{
}

void SemanticChecker::checkFunctionDeclared(const string& iden,
                                            list<string&> parametersTypes)
{
}

void SemanticChecker::checkModifiable(const string& iden)
{
}

string SemanticChecker::getFunctionType(const string& iden,
                                        const list<string& parametersTypes)
{
}

string SemanticChecker::getCurrentScope()
{
}

bool SemanticChecker::getIsInFor()
{
}

bool SemanticChecker::getIsMainPresent()
{
}

bool SemanticChecker::getIsInSwitch()
{
}

