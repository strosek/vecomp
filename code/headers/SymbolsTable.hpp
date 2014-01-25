#ifndef SYMBOLSTABLE_HPP
#define SYMBOLSTABLE_HPP

#include "SymbolData.hpp"

#include <map>

class SymbolsTable
{
public:
  SymbolsTable();

  bool         exists(std::string name);
  bool         isPresent(std::string name, std::string parameters) const;
  bool         isPresent(std::string name, std::string scope, size_t dimensions,
                         NativeType_t type) const;
  void         insert(std::string name, const SymbolData& data);
  NativeType_t getType(std::string name, std::string scope) const;
  NativeType_t getFunctionType(std::string name, std::string parameters) const;
  size_t       getDimensions(std::string name, std::string parameters) const;
  size_t       getFunctionDimensions(std::string name, 
                                     std::string parameters) const;

private:
  std::multimap<std::string, SymbolData> m_symbolsMap;
};

#endif /* SYMBOLSTABLE_HPP included */

