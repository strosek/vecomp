/* Definition of SemanticChecker class member functions */

#include "../headers/SemanticChecker.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

SemanticChecker::SemanticChecker(ErrorReporter * errorReporter) :
  m_controlStack(),
  m_errorReporter(errorReporter),
  m_forLevel(0),
  m_imports(),
  m_isMainPresent(false),
  m_isReturnCalled(false),
  m_isReturnRequired(false),
  m_switchLevel(0),
  m_symbolsTable(),
{
}

