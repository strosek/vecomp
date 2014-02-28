#ifndef SYMBOLSTABLE_HPP
#define SYMBOLSTABLE_HPP

#include "SymbolData.hpp"
#include "ErrorReporter.hpp"

#include <iostream>
#include <map>

class SymbolsTable
{
public:
  SymbolsTable();

  bool         exists(const std::string& name);
  void         checkDeclared(const std::string& name, const std::string& scope);
  void         checkFunctionDeclared(const std::string& name,
                                     const std::string& parameters);
  void         checkDeclarable(const std::string& name,
                               const std::string& scope);
  void         checkFunctionDeclarable(const std::string& name,
                                       const std::string& parameters);
  void         insert(const std::string& name, const SymbolData& data);
  NativeType_t getVariableType(const std::string& name,
                               const std::string& scope);
  NativeType_t getFunctionType(const std::string& name,
                               const std::string& parameters);
  size_t       getDimensions(const std::string& name,
                             const std::string& scope);
  size_t       getFunctionDimensions(const std::string& name, 
                                     const std::string& parameters);
  void         setErrorReporter(ErrorReporter* errorReporter);

  friend std::ostream& operator<<(std::ostream& out,
                                  const SymbolsTable& symbolsTable);

private:
  ErrorReporter*                         m_errorReporter;
  std::multimap<std::string, SymbolData> m_symbolsMap;

  std::pair<std::multimap<std::string, SymbolData>::iterator,
            std::multimap<std::string, SymbolData>::iterator> m_searchRange;
};

#endif /* SYMBOLSTABLE_HPP included */

