#ifndef SYMBOLSTABLE_HPP
#define SYMBOLSTABLE_HPP

#include "SymbolData.hpp"

#include <map>

class SymbolsTable
{
public:
  SymbolsTable();

  bool exists(std::string name);
  bool search(std::string name, std::string parameters) const;
  bool search(std::string name, std::string scope, size_t dimensions,
              NativeType_t type) const;
  void insert(std::string name, const SymbolData& data);

private:
  std::multimap<std::string, SymbolData> m_symbolsMap;
};

#endif /* SYMBOLSTABLE_HPP included */

