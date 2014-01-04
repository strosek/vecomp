/* Declaration of class SemanticChecker */

#ifndef SEMANTICCHECKER_HPP
#define SEMANTICCHECKER_HPP 

#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "../headers/TokenLexeme.hpp"
#include "../headers/ErrorReporter.hpp"

typedef enum
{
  TYPE_INTEGER,
  TYPE_FLOAT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID
} NativeType_t;

typedef std::list<std::pair<int, NativeType_t> > ParametersList_t;

typedef struct sd
{
  sd() :
    type(),
    dimensions(),
    isFunction(false),
    parametersList(),
    scope(),
    isConstant(false),
    line(1)
  {
  }

  NativeType_t     type;
  std::vector<int> dimensions;
  bool             isFunction;
  ParametersList_t parametersList;
  std::string      scope;
  bool             isConstant;
  size_t           line;
} SymbolData_t;


class SemanticChecker
{
public:
  SemanticChecker(ErrorReporter* errorReporter);
  SemanticChecker(const SemanticChecker& source);

  SemanticChecker& operator=(const SemanticChecker& rhs);

  void checkVariableDeclared(const TokenLexeme& token,
      const std::string& scope);
  void checkVariableNotDeclared(const TokenLexeme& token,
      const std::string& scope);
  void checkFunctionDeclared(const TokenLexeme& token,
                             ParametersList_t parametersList);
  void checkFunctionNotDeclared(const TokenLexeme& token,
                                ParametersList_t parametersList);
  void checkModifiable(const TokenLexeme& iden);
  void checkExpression(const std::string& expression, int line, int row);
  void checkDimensions(TokenLexeme& token, std::vector<int> sizes);

  std::string getTypeString(NativeType_t type);
  std::string getParametersString(ParametersList_t parametersList);
  std::string getFunctionType(const std::string& iden,
                              ParametersList_t parametersList);
  std::string getCurrentScope();
  bool        isInFor();
  bool        isMainPresent();
  bool        isInSwitch();

  void printSymbolsTable();

  void setMainPresent(bool isPresent);
  void enterFor();
  void exitFor();
  void enterSwitch();
  void exitSwitch();
  void enterToScope(const std::string& scope);
  void exitCurrentScope();
  void addImport(const TokenLexeme& import);
  void addSymbol(const std::string& name, SymbolData_t data);
  void addSymbols(
      const std::list<std::pair<std::string, SymbolData_t> > symbols);
private:
  void checkExpressionType(const std::string& expression,
      NativeType_t expectedType);
  bool isSymbolPresent(const std::string& name);
  bool parametersMatch(const std::string& name, 
                       ParametersList_t parametersList);

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

