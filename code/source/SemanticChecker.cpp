/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

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

bool SemanticChecker::isMainPresent() const
{
  return m_isMainPresent;
}
