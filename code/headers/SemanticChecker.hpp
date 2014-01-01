/* Declaration of class SemanticChecker */

#ifndef SEMANTICCHECKER_HPP
#define SEMANTICCHECKER_HPP 

#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "../headers/TokenLexeme.hpp"
#include "../headers/ErrorReporter.hpp"

typedef struct sd
{
  sd() :
    type(),
    dimensions(),
    parametersTypes(),
    returnType(),
    scope(),
    isConstant(false),
    line(1)
  {
  }

  std::string            type;
  std::vector<int>       dimensions;
  std::list<std::string> parametersTypes;
  std::string            returnType;
  std::string            scope;
  bool                   isConstant;
  size_t                 line;
} SymbolData_t;


class SemanticChecker
{
public:
  SemanticChecker(ErrorReporter* errorReporter);
  SemanticChecker(const SemanticChecker& source);

  SemanticChecker& operator=(const SemanticChecker& rhs);

  void        checkVariableDeclared(const TokenLexeme& token, 
                  const std::string& type, const std::string& scope);
  void        checkFunctionDeclared(const TokenLexeme& token,
                                    std::list<std::string> parametersTypes);
  void        checkModifiable(const TokenLexeme& iden);
  void        checkExpression(const std::string& expression, int line, int row);
  void        checkDimensions(TokenLexeme& token, std::vector<int> sizes);
  std::string getFunctionType(const std::string& iden,
                              std::list<std::string>& parametersTypes);

  std::string getCurrentScope();
  bool        isInFor();
  bool        isMainPresent();
  bool        isInSwitch();

  void        setMainPresent(bool isPresent);
  void        enterFor();
  void        exitFor();
  void        enterSwitch();
  void        exitSwitch();
  void        enterToScope(const std::string& scope);
  void        exitCurrentScope();
  void        addImport(const TokenLexeme& import);
private:
  void checkExpressionType(const std::string& expression,
                           const std::string& expectedType);
  bool isSymbolPresent(const std::string& name);
  bool parametersMatch(const std::string& name,
                       std::list<std::string>& parametersTypes);

  bool                                m_isMainPresent;
  int                                 m_forLevel;
  int                                 m_switchLevel;
  bool                                m_isReturnCalled;

  ErrorReporter *                     m_errorReporter;
  std::map<std::string, std::string>  m_expressionTypes;
  std::stack<std::string>             m_controlStack;
  std::set<std::string>               m_imports;
  std::map<std::string, SymbolData_t> m_symbolsTable;
};

#endif /* SEMANTICCHECKER_HPP */

