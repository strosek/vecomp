/* Declaration of class SemanticChecker */

#ifndef SEMANTICCHECKER_HPP
#define SEMANTICCHECKER_HPP 

#include <set>
#include <stack>
#include <string>

#include "ErrorReporter.hpp"
#include "SymbolsTable.hpp"
#include "TokenLexeme.hpp"

class SemanticChecker
{
public:
  SemanticChecker(ErrorReporter* errorReporter);
  SemanticChecker(const SemanticChecker& source);

  SemanticChecker& operator=(const SemanticChecker& rhs);

  void addImport(const std::string& import);
  void enterFor();
  void enterSwitch();
  void enterToScope(const std::string& scope);
  void exitCurrentScope();
  void exitFor();
  void exitSwitch();
  void setMainPresent(bool isPresent);

private:
  ErrorReporter *                    m_errorReporter;
  SymbolsTable                       m_symbolsTable;
  bool                               m_isMainPresent;
  bool                               m_isReturnCalled;
  bool                               m_isReturnRequired;
  int                                m_forLevel;
  int                                m_switchLevel;
  std::map<std::string, std::string> m_validExpressions;
  std::set<std::string>              m_imports;
  std::stack<std::string>            m_controlStack;
  std::stack<std::string>            m_expressionTypes;
};

#endif /* SEMANTICCHECKER_HPP */

