#ifndef SYMBOLSTABLE_HPP
#define SYMBOLSTABLE_HPP

#include "SymbolData.hpp"
#include "ErrorReporter.hpp"

#include <map>

class SymbolsTable
{
public:
  SymbolsTable();

  bool         exists(const std::string& name);
  void         checkDeclared(const std::string& name,
                             const std::string& parameters);
  void         checkDeclared(const std::string& name, const std::string& scope,
                          size_t dimensions, NativeType_t type);
  void         insert(const std::string& name, const SymbolData& data);
  NativeType_t getType(const std::string& name, const std::string& scope);
  NativeType_t getFunctionType(const std::string& name,
                               const std::string& parameters);
  size_t       getDimensions(const std::string& name,
                             const std::string& scope);
  size_t       getFunctionDimensions(const std::string& name, 
                                     const std::string& parameters);
  void         setErrorReporter(ErrorReporter* errorReporter);

private:
  ErrorReporter*                         m_errorReporter;
  std::multimap<std::string, SymbolData> m_symbolsMap;

  std::pair<std::multimap<std::string, SymbolData>::iterator,
            std::multimap<std::string, SymbolData>::iterator> m_searchRange;
};

#endif /* SYMBOLSTABLE_HPP included */

