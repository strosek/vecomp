#ifndef SYMBOLSTABLE_HPP
#define SYMBOLSTABLE_HPP

#include "Symbol.hpp"

class SymbolsTable
{
public:
  SymbolsTable();

  search(std::string name, SymbolData data);
private:
  std::multimap<std::string, SymbolData> m_symbolsMap;
};

#endif /* SYMBOLSTABLE_HPP included */
