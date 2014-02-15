/* Declaration of class SemanticChecker */

#ifndef SEMANTICCHECKER_HPP
#define SEMANTICCHECKER_HPP 

#include <set>
#include <stack>
#include <string>

#include "ErrorReporter.hpp"
#include "SymbolsTable.hpp"
#include "TokenLexeme.hpp"

const char OPERATOR_ARITH = 'A'; // * / % ^
const char OPERATOR_LOGIC = 'L'; // && ||
const char OPERATOR_MINUS = 'M'; // -
const char OPERATOR_NOT =   'N'; // !
const char OPERATOR_PLUS =  'P'; // +
const char OPERATOR_REL =   'R'; // < > <= >= != ==

class SemanticChecker
{
public:
  SemanticChecker();
  SemanticChecker(const SemanticChecker& source);

  SemanticChecker& operator=(const SemanticChecker& rhs);

  NativeType_t getExpressionType();
  bool         isInFor() const;
  bool         isInSwitch() const;
  bool         isMainPresent() const;
  void         addImport(const std::string& import);
  void         declare(SymbolData& data);
  void         checkExpressionType(NativeType_t expectedType);
  void         checkDeclared(const SymbolData& data);
  void         checkImported(const std::string& import);
  void         checkReturnRequired();
  void         enterFor();
  void         enterSwitch();
  void         enterToScope(const std::string& scope);
  void         exitCurrentScope();
  void         exitFor();
  void         exitSwitch();
  void         pushOperand(char op);
  void         pushOperator(char op);
  void         setMainPresent(bool isPresent);
  void         setReturnCalled(bool isCalled);
  void         setReturnRequired(bool isRequired);

  void         printSymbolsTable() const;
  void         setErrorReporter(ErrorReporter* errorReporter);

private:
  ErrorReporter *             m_errorReporter;
  SymbolsTable                m_symbolsTable;
  bool                        m_isMainPresent;
  bool                        m_isReturnCalled;
  bool                        m_isReturnRequired;
  int                         m_forLevel;
  int                         m_switchLevel;
  std::map<std::string, char> m_validExpressions;
  std::set<std::string>       m_imports;
  std::stack<std::string>     m_controlStack;
  std::stack<char>            m_operations;
};

#endif /* SEMANTICCHECKER_HPP */

