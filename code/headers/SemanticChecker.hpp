/* Declaration of class SemanticChecker */

#ifndef SEMANTICCHECKER_HPP
#define SEMANTICCHECKER_HPP 

#include <list>
#include <map>
#include <string>

typedef struct st
{
  std::string type;
  int    dimensions;
  std::list<std::string&> parametersTypes;
} SymbolData_t;

class SemanticChecker
{
public:
  SemanticChecker();

  void        checkVariableDeclared(const std::string& iden);
  void        checkFunctionDeclared(const std::string& iden,
                                 list<std::string&> parametersTypes);
  void        checkModifiable(const std::string& iden);
  std::string getFunctionType(const std::string& iden,
                              const list<std::string>& parametersTypes);
  std::string getCurrentScope();
  bool        getIsInFor();
  bool        getIsMainPresent();
  bool        getIsInSwitch();
private:
  void        getExpressionType(const std::string& expression,
                                const std::string& expectedType);

  bool                                 m_isMainPresent;
  bool                                 m_isInFor;
  bool                                 m_isInSwitch;
  bool                                 m_isReturnCalled;

  ErrorReporter *                      m_errorReporter;
  std::map<std::string&, std::string&> m_expressionTypes;
  std::stack<std::string>              m_controlStack;
  std::map<std::string&, SymbolData_t> m_symbolsTable;
};

#endif /* SEMANTICCHECKER_HPP */

