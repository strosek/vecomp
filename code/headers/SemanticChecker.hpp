/* Declaration of class SemanticChecker */

#ifndef SEMANTICCHECKER_HPP
#define SEMANTICCHECKER_HPP 

#include <set>
#include <stack>
#include <string>

#include "../headers/TokenLexeme.hpp"
#include "../headers/ErrorReporter.hpp"

class SemanticChecker
{
public:
  SemanticChecker(ErrorReporter* errorReporter);
  SemanticChecker(const SemanticChecker& source);

  SemanticChecker& operator=(const SemanticChecker& rhs);

  void printSymbolsTable();

  void setMainPresent(bool isPresent);
  void enterFor();
  void exitFor();
  void enterSwitch();
  void exitSwitch();
  void enterToScope(const std::string& scope);
  void exitCurrentScope();
  void addImport(const std::string& import);
private:
  bool                                m_isMainPresent;
  int                                 m_forLevel;
  int                                 m_switchLevel;
  bool                                m_isReturnCalled;
  bool                                m_isReturnRequired;

  ErrorReporter *                     m_errorReporter;
  std::stack<std::string>             m_controlStack;
  std::set<std::string>               m_imports;
  SymbolsTable                        m_symbolsTable;
};

#endif /* SEMANTICCHECKER_HPP */

