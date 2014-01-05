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

  void checkVariableDeclared(const TokenLexeme& token);
  void checkVariableNotDeclared(const TokenLexeme& token);
  void checkFunctionDeclared(const TokenLexeme& token,
                             ParametersList_t parametersList);
  void checkFunctionNotDeclared(const TokenLexeme& token,
                                ParametersList_t parametersList);
  void checkModifiable(const TokenLexeme& iden);
  void checkExpression(const std::string& lexeme, int line, int row);
  void checkDimensions(TokenLexeme& token, std::vector<int> sizes);
  void checkReturnType(TokenLexeme& token);
  void checkReturnShouldBeCalled(int functionEndLine);
  void checkImported(const std::string& package, int line, int row);

  std::string getTypeString(NativeType_t type);
  std::string getParametersString(ParametersList_t parametersList);
  std::string getFunctionType(const std::string& iden,
                              ParametersList_t parametersList);
  std::string appendCurrentScope(std::string name);
  std::string appendGlobalScope(std::string name);
  std::string getActualSymbol(std::string iden, bool isFunction);
  std::string getCurrentScope();
  bool        isInFor();
  bool        isMainPresent();
  bool        isInSwitch();
  bool        isReturnCalled();

  void printSymbolsTable();

  void setMainPresent(bool isPresent);
  void setReturnCalled(bool isCalled);
  void setReturnRequired(bool isRequired);
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
  void pushOperand(TokenLexeme& token);
  void pushOperand(TokenLexeme& token, ParametersList_t parameters);
  void pushOperator(char op);
private:
  void checkExpressionType(const std::string& expression,
      NativeType_t expectedType);
  bool isSymbolPresent(const std::string& name);
  bool parametersMatch(const std::string& name, 
                       ParametersList_t parametersList);
  char getTypeChar(NativeType_t type);

  bool                                m_isMainPresent;
  int                                 m_forLevel;
  int                                 m_switchLevel;
  bool                                m_isReturnCalled;
  bool                                m_isReturnRequired;

  ErrorReporter *                     m_errorReporter;
  std::map<std::string, char>         m_expressionTypes;
  std::stack<std::string>             m_controlStack;
  std::set<std::string>               m_imports;
  std::map<std::string, SymbolData_t> m_symbolsTable;
  std::stack<char>                    m_typesStack;
};

#endif /* SEMANTICCHECKER_HPP */

